//
// Created by adame on 12/8/2024.
//

#include "parser_utils.h"

#include <cstddef>
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
    for (const auto& item : free_registers) {
            printf("free regs : from %d to %d\n", item.first, item.second);
    }
    return reg;
}

register_table::register_table() : free_registers(), table() {
    free_registers.insert(std::make_pair(1,2<<10));
}

void register_table::remove(const std::string& pid) {
    pid_type pid_t = table.at(pid);
    int from = pid_t.register_no;
    int to = from + pid_t.size;
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
        //TODO report pid redeclaration
        return -1;
    }

    pid_type new_pid;
    new_pid.size = 1;
    new_pid.register_no = available_register();

    table[pid] = new_pid;
    return new_pid.register_no;
}

void register_table::add_table(const std::string& pid, int from, int to) {

}
