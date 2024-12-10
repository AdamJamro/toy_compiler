//
// Created by adame on 12/8/2024.
//
#include "parser_utils.h"
#include <functional>
#include <stdexcept>
#include <unordered_set>
#include <utility>

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
