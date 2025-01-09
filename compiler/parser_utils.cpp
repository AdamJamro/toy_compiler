//
// Created by adame on 12/8/2024.
//
#include "parser_utils.h"
#include <functional>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <algorithm>

void postprocess(const std::string& filename, register_table& regs) {
    std::ifstream input_file(filename);
    std::list<std::string> file_contents;
    std::string line;

    while (std::getline(input_file, line)) {
        file_contents.emplace_back(line);
    }
    input_file.close();

    std::ofstream output_file(filename);


    // calculate header offset and collect all cached constants
    auto header_offset = 0;
    std::unordered_set<long> cached_constants = {};
    for (const auto& translation_line : file_contents) {
        check_for_caches(translation_line, cached_constants);
        if (translation_line.find("this_line")!= std::string::npos) {
            header_offset += 2;
        }
    }
    if (file_contents.front().compare(0, 4, "JUMP") == 0) {
        header_offset += 1;
    }


    auto cache_reg = regs.unused_register(); // outputs registers never yet returned by any add/assign method
    std::unordered_map<std::string, long> cache_regs = {};
    for (auto const constant : cached_constants) {
        // TODO cache uncached [constants]
        // TODO un-cache unused constants

        // take care of cached constants:
        cache_regs[std::to_string(constant)] = cache_reg;
        output_file << "SET " << constant << std::endl;
        //regs.add(to_string(constant), cache_register); //???
        //output_file << "STORE [" << constant << "]" << endl; // delete
        output_file << "STORE " << cache_reg << "\t# ["<< constant << "]" << std::endl;
        //translation_header_offset += 2;
        cache_reg++; // use next register
    }

    auto header_size = cache_regs.size();
    long line_count = 0;
    for (auto& translation_line : file_contents) {
        parse_line(translation_line, line_count, header_size, cache_regs);
        line_count++;
    }

    for (auto& translation_line : file_contents) {
        output_file << translation_line << std::endl;
    }

}


TokenAttribute* parse_expression(TokenAttribute* token1, TokenAttribute* token2, const std::string& operation, const std::string& reverse_operation, const long value, const long tmp_reg) {
    // TODO probably reverse_operation is not required for parsing expression
    // TODO cache constants

    if (token1->type == ADDRESS || token2->type == ADDRESS) {
        const auto swap = token1->type != ADDRESS;
        auto * const adr_token = swap ? token2 : token1;
        auto * const other_token = swap ? token1 : token2;
        adr_token->translation.emplace_back("LOADI 0");

        if (other_token->type == ADDRESS) {
            // ADDRESS .. ADDRESS
            // 1st address already in r0
            adr_token->translation.emplace_back("STORE " + std::to_string(tmp_reg));
            adr_token->translation.splice(adr_token->translation.end(), other_token->translation);
            adr_token->translation.emplace_back("LOADI 0");
            adr_token->translation.emplace_back((swap ? reverse_operation : operation) + " " + std::to_string(tmp_reg));
        } else if (other_token->type == STRING) {
            // ADDRESS .. PID
            adr_token->translation.emplace_back((swap ? reverse_operation : operation) +  " " + std::to_string(other_token->register_no));
        } else if (other_token->type == LONG) {
            // ADDRESS .. RVAL
            adr_token->translation.emplace_back((swap ? reverse_operation : operation) + " [" + std::to_string(other_token->long_value) + "]");
        } else {
            throw std::invalid_argument("invalid value type inside of expression");
        }
        token1 = adr_token; // free token2 reuse token1 and return it
        free(other_token);
    } else if (token1->type == token2->type) {
        if (token1->type == STRING) {
            // PID .. PID
            token1->translation.emplace_back("LOAD " + std::to_string(token1->register_no));
            token1->translation.emplace_back(operation + " " + std::to_string(token2->register_no));
        } else {
            // RVAL .. RVAL
            token1->translation.emplace_back("SET " + std::to_string(value));
        }
        free(token2);
    } else {
        // RVAL + PID
        const auto swap = token1->type == STRING;
        TokenAttribute* str_token = swap ? token1 : token2;
        TokenAttribute* num_token = swap ? token2 : token1;

        str_token->translation.emplace_back("LOAD [" + std::to_string(num_token->long_value) + "]");
        str_token->translation.emplace_back((swap ? reverse_operation : operation) + " " + std::to_string(str_token->register_no));
        token1 = str_token; // ensure $$ has the rval register and string type
        free(num_token);
    }

    token1->str_value = "rval"; // TODO it may be useless
    token1->register_no = 0; // RESULT OF THE EXPRESSION STORED IN R0!
    return token1;
    //EXCESS TOKEN ALREADY CLEANED UP!
}

TokenAttribute* parse_condition(TokenAttribute* token1, TokenAttribute* token2, std::list<std::string> condition_translation, const std::list<std::string> &inverse_condition_translation, const bool logic_value, const int tmp_register, const int lineno, std::unordered_set<long>& cached_constants) {

    if (token1->type == ADDRESS || token2->type == ADDRESS) {
        const auto swap = token1->type != ADDRESS;
        auto * const adr_token = swap ? token2 : token1;
        auto * const other_token = swap ? token1 : token2;

        if (other_token->type == ADDRESS) {
            adr_token->translation.emplace_back("LOADI 0");
            adr_token->translation.emplace_back("STORE " + std::to_string(tmp_register));
            adr_token->translation.splice(adr_token->translation.end(),  other_token->translation);
            adr_token->translation.emplace_back("LOADI 0");
            adr_token->translation.emplace_back("SUB " + std::to_string(tmp_register));
            token1->translation = adr_token->translation; // we reuse this token1 and return it later
        } else if (other_token->type == STRING) {
            adr_token->translation.emplace_back("LOADI 0");
            adr_token->translation.emplace_back("SUB " + std::to_string(other_token->register_no));
            token1->translation = adr_token->translation;
        } else if (other_token->type == LONG) {
            // adr_token already put its addres in r0
            adr_token->translation.emplace_back("LOADI 0");
            if (other_token->long_value != 0) {
                adr_token->translation.emplace_back("SUB [" + std::to_string(other_token->long_value) + "]"); // todo notify main about new cached const?
            }
            token1->translation = adr_token->translation;
        } else {
            throw std::invalid_argument("invalid token type");
        }
        if (swap) {
            condition_translation = inverse_condition_translation;
        }
        token1->type = STRING;
    } else if (token1->type == token2->type) {
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

        auto const * const str_token = swap_needed? token1 : token2;
        auto const * const rval_token = swap_needed? token2 : token1;

        if (rval_token->long_value != 0) {
            token1->translation.emplace_back("SET [" + std::to_string(rval_token->long_value) + "]");
            token1->translation.emplace_back("SUB " + std::to_string(str_token->register_no));
            cached_constants.insert(rval_token->long_value);
        } else {
            swap_needed = !swap_needed;
            token1->translation.emplace_back("LOAD " + std::to_string(str_token->register_no));
        }

        if (swap_needed) {
            // swap was needed -> so we need to swap logic;
            condition_translation = inverse_condition_translation;
        }
        token1->type = STRING;
    }

    token1->translation.splice(token1->translation.end(), condition_translation);
    token1->register_no = 0; // RESULT OF THE CONDITION STORED IN R0!

    free(token2);
    return token1;
}

void check_for_caches(const std::string& line, std::unordered_set<long>& cached_constants) {
    if (const auto pos = line.find("this_line"); pos != std::string::npos) {
        return;
    }
    if (const auto pos = line.find("["); pos != std::string::npos) {
        if (const auto comment_pos = line.find("#"); comment_pos != std::string::npos && comment_pos < pos) {
            return; // it's a comment
        }
        const auto end_pos = line.find("]", pos);
        const auto val_length = end_pos - pos - 1;
        const auto& cached_value = line.substr(pos + 1, val_length);
        cached_constants.insert(std::stol(cached_value));
        std::cout << "cached value: " << cached_value << std::endl;
    }
}

void parse_line(std::string& line, const long line_no, const long header_offset, std::unordered_map<std::string, long>& cache_regs) {

    //replace this_line with actual line_no
    if (const auto pos = line.find("this_line"); pos != std::string::npos) {
        if (const auto pos_proc_call_load_line = line.find("[this_line + 3]"); pos_proc_call_load_line != std::string::npos) {
            line.replace(pos_proc_call_load_line + 1, 13, std::to_string(line_no + 3));
        }
        if (const auto pos_proc_call_jump_to_proc = line.find("JUMP "); pos_proc_call_jump_to_proc != std::string::npos) {
            const int value_length = line.find(" -") - line.front();
            const auto value = std::stoi(line.substr(pos_proc_call_jump_to_proc + 4, value_length));
            line = "JUMP " + std::to_string(value - line_no + header_offset) + "\t# original: " + std::to_string(value) + " - " + std::to_string(line_no);
        }
        return;
    }

    //replace [cached] value with its reg
    if (const auto pos = line.find("["); pos != std::string::npos) {
        if (const auto comment_pos = line.find("#"); comment_pos != std::string::npos && comment_pos < pos) {
            return; // it's a comment
        }
        const auto end_pos = line.find("]", pos);
        const auto val_length = end_pos - pos - 1;
        const auto& cached_value = line.substr(pos + 1, val_length);
        std::cout << "line: " << line << std::endl;
        std::cout << "cached value: " << cached_value << std::endl;
        line.replace(pos, val_length + 2, std::to_string(cache_regs.at(cached_value)));
        line.append("\t# [" + cached_value + "]");
    }
}


void parse_proc_line(std::string& line, const std::list<long>& arg_regs) {

    for (const auto& reg : arg_regs) {
        const auto& reg_str = std::to_string(reg);
        if (const auto pos = line.find(reg_str); pos != std::string::npos) {

            if (line.compare(0, 4, "LOAD") == 0) {
                if (line.find("# passing address") != std::string::npos) {
                    // if it was reference pass we pass it as is
                    // if it was a proc call then erase brackets
                    const auto close_bracket_pos = line.find(']');
                    const auto open_bracket_pos = line.find('[');
                    if (close_bracket_pos != std::string::npos && open_bracket_pos != std::string::npos) {
                        line.erase(close_bracket_pos, 1);
                        line.erase(open_bracket_pos, 1);
                    }
                    // if it wasn't then we just leave it
                } else {
                    if (line.find("[") != std::string::npos) {
                        return; // the arg was used as a constant
                    }
                    line = "LOADI " + reg_str;
                }

                continue;
            }

            // LOAD was the only case that could interfere with [ ] constants
            if (line.find("[") != std::string::npos) {
                return; // the arg was used as a constant
            }

            if (line.compare(0, 5, "LOADI") == 0) {
                line = "LOADI " + reg_str + "\nLOADI 0";
            } else if (line.compare(0, 5, "STORE") == 0) {
                line = "STOREI " + reg_str;
            } else if (line.compare(0, 6, "STOREI") == 0) {
                line = "STOREII " + reg_str;
            } else if (line.compare(0, 3, "ADD") == 0) {
                line = "ADDI " + reg_str;
            } else if (line.compare(0, 4, "ADDI") == 0) {
                line = "ADDII " + reg_str;
            } else if (line.compare(0, 3, "SUB") == 0) {
                line = "SUBI " + reg_str;
            } else if (line.compare(0, 4, "SUBI") == 0) {
                line = "SUBII " + reg_str;
            } else if (line.compare(0, 3, "MUL") == 0) {
                line = "MULI " + reg_str;
            } else if (line.compare(0, 3, "DIV") == 0) {
                line = "DIVI " + reg_str;
            } else if (line.compare(0, 3, "MOD") == 0) {
                line = "MODI " + reg_str;
            } else if (line.compare(0, 3, "GET") == 0) {
                line = "GET 0\nSTOREI " + reg_str;
            } else if (line.compare(0, 3, "PUT") == 0) {
                line = "LOADI " + reg_str + "\nPUT 0";
            }
        }
    }
}


//FUNS_TABLE
void funs_table::add(const std::string &fun_name, long line_no, long amount_of_arguments, long first_reg) {
    line_no_of_[fun_name] = line_no;
    arg_count_of_[fun_name] = amount_of_arguments;
    first_arg_register_of_[fun_name] = first_reg;
}

long funs_table::get_line_no(const std::string &fun_name) const {
    return line_no_of_.at(fun_name);
}

long funs_table::get_arg_count(const std::string &fun_name) const {
    return arg_count_of_.at(fun_name);
}

long funs_table::get_first_arg_register(const std::string &fun_name) const {
    return first_arg_register_of_.at(fun_name);
}

//REGISTER_TABLE
int register_table::assign_register() {
    return assign_registers(1);

    // deprecated
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

int register_table::assign_registers(const int size) {
    if (free_registers.empty()) {
        throw std::runtime_error("MEMORY ERROR: OUT OF REGISTERS");
    }
    // std::cout << "assignment size: "<<size<<std::endl<<"free regs before oparation:" << std::endl;
    // for (const auto& [first, last] : free_registers) {
    //     printf("from %lld to %lld\n", first, last);
    // }

    auto it = free_registers.begin(); // first free interval of registers
    const auto end = free_registers.end();
    while (it != end && it->second - it->first < size) {
        ++it;
        if (it == free_registers.begin())
            throw std::runtime_error("ure bad programmer."); // delete
    }
    if (it == end) {
        throw std::runtime_error("FRAGMENTATION FAULT: OUT OF REGISTERS");
    }
    const auto reg = it->first;
    free_registers.erase(it);
    if (it->second - it->first > size) {
        const auto chopped_range = std::make_pair(it->first + size, it->second);
        free_registers.insert(chopped_range);
    }

    // std::cout << "free regs after oparation:" << std::endl;
    // for (const auto& [first, last] : free_registers) {
    //     printf("from %lld to %lld\n", first, last);
    // }

    // last register is the first unused by the assigment [first, last)
    first_untouched_register = std::max(first_untouched_register, static_cast<long>(it->first + size));
    return reg;
}

register_table::register_table() : free_registers(), table() {
    free_registers.insert(std::make_pair(1,2<<29));
    // r0 is used as a special temporary register
}

void register_table::remove(const std::string& pid) { // maybe free_pid() ?
    auto [size, reg, _] = table.at(pid);
    int from = reg;
    int to = from + size;
    auto range = std::make_pair(from, to);

    auto lower_bound = free_registers.lower_bound(range);
    lower_bound = (lower_bound == free_registers.begin())? lower_bound : --lower_bound;
    const auto upper_bound = free_registers.upper_bound(range);
    //std::cout << "OOUOUOUOUO : Lb.1: " << lower_bound->first << ", Lb.2: " << lower_bound->second <<", Ub.1: " << upper_bound->first <<", Ub.2: " <<upper_bound->second << std::endl;
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

    // std::cout << "free regs after remove pid:" << pid <<" : " << std::endl ;
    // for (const auto& [first, last] : free_registers) {
    //     printf("regs : from %lld to %lld\n", first, last);
    // }
}

void register_table::remove(const int reg) { // maybe free_pid() ?

    int from = reg;
    int to = reg + 1;
    auto range = std::make_pair(from, to);
    auto lower_bound = free_registers.lower_bound(range);
    lower_bound = lower_bound == free_registers.begin()? lower_bound : --lower_bound;
    const auto upper_bound = free_registers.upper_bound(range);

    // std::cout << "hitched at: Lb.1: " << lower_bound->first << ", Lb.2: " << lower_bound->second <<", Ub.1: " << upper_bound->first <<", Ub.2: " <<upper_bound->second << std::endl;
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
    // std::cout << "free regs after remove pid:" << reg <<" : " << std::endl ;
    // for (const auto& [first, last] : free_registers) {
    //     printf("free regs : from %lld to %lld\n", first, last);
    // }
}

[[maybe_unused]]
void register_table::forget_pid(const std::string& pid) {
    table.erase(pid);
}

int register_table::at(const std::string& pid) const {
    return table.at(pid).register_no;
}

int register_table::at(const std::string& pid, const int index) const {
    return table.at(pid).at(index);
}

int register_table::add_rval(void) { // maybe rename to add_tmp or get_free_register
    if (free_registers.empty()) {
        throw std::runtime_error("ERROR: OUT OF REGISTERS");
    }
    const auto it = free_registers.begin();
    const auto reg = it->first;

    first_untouched_register = std::max(first_untouched_register, static_cast<long>(reg + 1));
    return reg;
}

pid_type register_table::get_pid(const std::string& pid) const {
    return table.at(pid);
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
    new_pid.register_no = assign_register();

    table[pid] = new_pid;
    return new_pid.register_no;
}

int register_table::add() { // WARNING IT NEVER FREES THE REGISTER
    pid_type new_pid;
    new_pid.size = 1;
    new_pid.register_no = assign_register();

    return new_pid.register_no;
}

void register_table::add_cache_reg(const std::string &pid, const long long reg) {
    if (table.contains(pid)){
        throw std::runtime_error("Parser error: wrong usage of cache register declaration");
    } //TODO it was simply commented out you need to handle it in the parser tho

    // it's the last step of the program there's no need to update free_registers
    pid_type new_pid;
    new_pid.size = 1;
    new_pid.register_no = reg;
    table[pid] = new_pid;
}

int register_table::add_table(const std::string &pid, const int from, const int to) {
    if (table.contains(pid)){
        // throw std::runtime_error("Syntax Error: Redeclaration of variable");
        return table[pid].register_no;
    } //TODO it was simply commented out you need to handle it in the parser tho

    pid_type new_pid;
    new_pid.size = to - from + 1;
    new_pid.index_shift = from;
    new_pid.register_no = assign_registers(new_pid.size);
    if (new_pid.size <= 0) {
        throw std::invalid_argument("this program cannot declare insideout tables!");
    }
    table[pid] = new_pid;
    return new_pid.register_no;
}

int register_table::add_proc_table(const std::string& pid) {
    if (table.contains(pid)){
        // throw std::runtime_error("Syntax Error: Redeclaration of variable");
        return table[pid].register_no;
    } //TODO it was simply commented out you need to handle it in the parser tho

    pid_type new_pid;
    // mock a table
    // make 0 a special value size for this case
    new_pid.size = 0;
    new_pid.index_shift = 0;
    new_pid.register_no = assign_register();
    table[pid] = new_pid;
    return new_pid.register_no;
}

//returns first free register that was never assigned to any pid or tid
long register_table::unused_register() const {
    return first_untouched_register;
}


//MISCELLANEOUS
std::string extract_function_name(const std::string& fun_call) {
    const size_t pos = fun_call.find('(');
    if (pos == std::string::npos) {
        throw std::invalid_argument("wrong number of arguments! \nin function call: " + fun_call);
    }
    return fun_call.substr(0, pos);
}
