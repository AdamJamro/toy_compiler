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
            std::cerr << "WARNING on line: " << lineno << " condition always " << (logic_value ? "TRUE" : "FALSE") << std::endl;
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

void register_table::remove(const std::string& pid) { // maybe free_pid() ?
    auto [size, reg] = table.at(pid);
    int from = reg;
    int to = from + size;
    auto range = std::make_pair(from, to);

    auto lower_bound = free_registers.lower_bound(range);
    lower_bound = lower_bound == free_registers.begin()? lower_bound : --lower_bound;
    const auto upper_bound = free_registers.upper_bound(range);
    std::cout << "OOUOUOUOUO : Lb.1: " << lower_bound->first << ", Lb.2: " << lower_bound->second <<", Ub.1: " << upper_bound->first <<", Ub.2: " <<upper_bound->second << std::endl;
    if (lower_bound->second < reg || reg < upper_bound->first - 1) {
        free_registers.insert(range);
    } else if (lower_bound->second + 1 == upper_bound->first) {
        range.first = lower_bound->first;
        range.second = upper_bound->second;
        free_registers.erase(lower_bound);
        free_registers.erase(upper_bound);
        free_registers.insert(range);
    } else if (lower_bound->second == reg) {
        range.first = lower_bound->first;
        free_registers.erase(lower_bound);
        free_registers.insert(range);
    } else if (upper_bound->first - 1 == reg) {
        range.second = upper_bound->first;
        free_registers.erase(upper_bound);
        free_registers.insert(range);
    }

    table.erase(pid);

    std::cout << "free regs after remove pid:" << pid <<" : " << std::endl ;
    for (const auto& [first, last] : free_registers) {
        printf("regs : from %lld to %lld\n", first, last);
    }
}

void register_table::remove(const int reg) { // maybe free_pid() ?

    int from = reg;
    int to = reg + 1;
    auto range = std::make_pair(from, to);
    auto lower_bound = free_registers.lower_bound(range);
    lower_bound = lower_bound == free_registers.begin()? lower_bound : --lower_bound;
    const auto upper_bound = free_registers.upper_bound(range);

    std::cout << "OOUOUOUOUO : Lb.1: " << lower_bound->first << ", Lb.2: " << lower_bound->second <<", Ub.1: " << upper_bound->first <<", Ub.2: " <<upper_bound->second << std::endl;
    if (lower_bound->second < reg || reg < upper_bound->first - 1) {
        free_registers.insert(range);
    } else if (lower_bound->second + 1 == upper_bound->first) {
        range.first = lower_bound->first;
        range.second = upper_bound->second;
        free_registers.erase(lower_bound);
        free_registers.erase(upper_bound);
        free_registers.insert(range);
    } else if (lower_bound->second == reg) {
        range.first = lower_bound->first;
        free_registers.erase(lower_bound);
        free_registers.insert(range);
    } else if (upper_bound->first - 1 == reg) {
        range.second = upper_bound->first;
        free_registers.erase(upper_bound);
        free_registers.insert(range);
    }
    std::cout << "free regs after remove pid:" << reg <<" : " << std::endl ;
    for (const auto& [first, last] : free_registers) {
        printf("free regs : from %lld to %lld\n", first, last);
    }
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

int register_table::contains(const std::string& pid) const {
    return table.contains(pid);
}

int register_table::add(const std::string& pid) {
    if (table.contains(pid)){
        // throw std::runtime_error("Syntax Error: Redeclaration of variable");
        return table[pid].register_no;
    } //TODO it was simply commented out you need to handle it in the parser tho

    pid_type new_pid;
    new_pid.size = 1;
    new_pid.register_no = available_register();

    table[pid] = new_pid;
    return new_pid.register_no;
}

int register_table::add() { // WARNING IT NEVER FREES THE REGISTER
    pid_type new_pid;
    new_pid.size = 1;
    new_pid.register_no = available_register();

    return new_pid.register_no;
}

void register_table::add_table(const std::string& pid, int from, int to) {
    throw std::runtime_error("NO TABLE SUPPORT IMPLEMENTED!");
}
