//
// Created by adame on 12/8/2024.
//
#include "parser_utils.h"
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <utility>

TokenAttribute* parse_condition(TokenAttribute* token1, TokenAttribute* token2, std::list<std::string> condition_translation, const std::list<std::string> &inverse_condition_translation, const bool logic_value, const int lineno) {

    if (token1->type == token2->type) {
        if (token1->type == STRING) {
            // PID ? PID
            token1->translation.emplace_back("LOAD " + std::to_string(token1->register_no));
            token1->translation.emplace_back("SUB " + std::to_string(token2->register_no));
        } else {
            // rVAL ? rVAL
            token1->type = LONG;
            token1->long_value = logic_value;
            std::cerr << "WARNING line: " << lineno << " condition always " << (logic_value ? "TRUE" : "FALSE") << std::endl;
        }
    } else {
        // PID ? rVAL
        // we need to put rvalue on the left side of the condition
        bool swap_needed = token1->type == STRING;
        if (swap_needed) {
            // swap was needed -> so we swap logic;
            condition_translation = inverse_condition_translation;
        }
        const auto* str_token = swap_needed? token1 : token2;
        const auto* rval_token = swap_needed? token2 : token1;

        token1->translation.emplace_back("SET " + std::to_string(rval_token->long_value));
        token1->translation.emplace_back("SUB " + std::to_string(str_token->register_no));

        token1->type = STRING;
    }

    token1->translation.splice(token1->translation.end(), condition_translation);
    token1->register_no = 0; // RESULT OF THE CONDITION STORED IN R0!
    free(token2);
    return token1;
}

//REGISTER_TABLE
int register_table::available_register(void) {
    if (free_registers.empty()) {
        throw std::runtime_error("ERROR OUT OF REGISTERS");
    }
    const auto it = free_registers.begin();
    const auto reg = it->first;

    if (it->first + 1 < it->second) {
        free_registers.insert(std::make_pair(it->first + 1, it->second));
    }
    free_registers.erase(it);
    for (const auto& [first, last] : free_registers) {
            printf("free regs : from %lld to %lld\n", first, last);
    }
    return reg;
}

register_table::register_table() : free_registers(), table() {
    free_registers.insert(std::make_pair(1,2<<29));
}

void register_table::remove(const std::string& pid) {
    auto [size, register_no] = table.at(pid);
    int from = register_no;
    int to = from + size;
    auto range = std::make_pair(from, to);

    const auto lower_bound = free_registers.lower_bound(range);
    const auto upper_bound = free_registers.upper_bound(range);
    if (lower_bound->second == from) {
        range.first = lower_bound->first;
        free_registers.erase(lower_bound);
    }
    if (upper_bound->first == to) {
        range.second = upper_bound->first;
        free_registers.erase(upper_bound);
    }

    free_registers.insert(std::make_pair(from, to));
    table.erase(pid);
}

int register_table::at(const std::string& pid) const {
    return table.at(pid).register_no;
}

int register_table::add_rval(void) const {
    if (free_registers.empty()) {
        throw std::runtime_error("ERROR OUT OF REGISTERS");
    }
    const auto it = free_registers.begin();
    const auto reg = it->first;

    return reg;
}

int register_table::add(const std::string& pid) {
    if (table.contains(pid)){
        throw std::runtime_error("Syntax Error: Redeclaration of variable");
    }

    pid_type new_pid;
    new_pid.size = 1;
    new_pid.register_no = available_register();

    table[pid] = new_pid;
    return new_pid.register_no;
}

void register_table::add_table(const std::string& pid, int from, int to) {
    throw std::runtime_error("NO TABLE SUPPORT IMPLEMENTED!");
}
