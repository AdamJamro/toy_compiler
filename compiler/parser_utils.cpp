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
#include <regex>

const std::string math_module::multiplication_path = "../compiler/math_module/multiplication.mr";
const std::string math_module::division_path = "../compiler/math_module/division.mr";

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
            adr_token->translation.emplace_back(reverse_operation + " " + std::to_string(tmp_reg));
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
            token1->translation.emplace_back("LOAD [" + std::to_string(value) + "]");
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
            token1->translation.emplace_back("LOAD [" + std::to_string(rval_token->long_value) + "]");
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

// returns (for_header, for_footer)
std::pair<std::list<std::string>, std::list<std::string>> parse_for_loop(const TokenAttribute* token1, const TokenAttribute* token2, const long tid_iterator_register, const long loop_body_size, std::unordered_set<long>& cached_constants, register_table& regs, const bool fast_loop) {
    std::string CONDITIONAL_JUMP; // used to remember if we did v1 - v2 or v2 - v1 to decide when to stop the loop in the footer
    std::list<std::string> for_header;
    std::list<std::string> for_footer;
    long tid_upper_limit_register;

    // create for_head
    if (token1->type == LONG && token2->type == LONG) { // SPECIAL CASE [rVAL, rVAL] can be optimized like:
        if (fast_loop) {
            for_header = {
                 "LOAD [" + std::to_string(token1->long_value - token2->long_value) + "]", // store only one value v == v1-v2 <= 0
                 "STORE " + std::to_string(tid_iterator_register),
            };
            CONDITIONAL_JUMP = "JPOS";
            cached_constants.insert(token1->long_value - token2->long_value);
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = {
                "LOAD [" + std::to_string(token1->long_value) + "]",
                "STORE " + std::to_string(tid_iterator_register),
                "LOAD [" + std::to_string(token2->long_value) + "]",
                "STORE " + std::to_string(tid_upper_limit_register),
                "SUB " + std::to_string(tid_iterator_register),
            };
            cached_constants.insert(token1->long_value);
            cached_constants.insert(token2->long_value);
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == LONG && token2->type == STRING) {
        if (fast_loop) {
            for_header = {
            "LOAD ["+ std::to_string(token1->long_value) + "]",
            "SUB "  + std::to_string(token2->register_no),
            "STORE "+ std::to_string(tid_iterator_register),
            };
            CONDITIONAL_JUMP = "JPOS";
            cached_constants.insert(token1->long_value);
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = {
                "LOAD [" + std::to_string(token1->long_value) + "]",
                "STORE " + std::to_string(tid_iterator_register),
                "LOAD " + std::to_string(token2->register_no),
                "STORE " + std::to_string(tid_upper_limit_register),
                "SUB " + std::to_string(tid_iterator_register),
            };
            cached_constants.insert(token1->long_value);
            cached_constants.insert(token2->long_value);
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == STRING && token2->type == LONG) {
        if (fast_loop) {
            for_header = {
                "LOAD [" + std::to_string(token2->long_value) + "]",
                "SUB "   + std::to_string(token1->register_no),
                "STORE " + std::to_string(tid_iterator_register),
            };
            CONDITIONAL_JUMP = "JNEG";
            cached_constants.insert(token2->long_value);
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = {
                "LOAD " + std::to_string(token1->register_no),
                "STORE " + std::to_string(tid_iterator_register),
                "LOAD [" + std::to_string(token2->long_value) + "]",
                "STORE " + std::to_string(tid_upper_limit_register),
                "SUB " + std::to_string(tid_iterator_register),
            };
            cached_constants.insert(token2->long_value);
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == STRING && token2->type == STRING) {
        if (fast_loop) {
            for_header = {
                "LOAD " + std::to_string(token2->register_no),
                "SUB "  + std::to_string(token1->register_no),
                "STORE "+ std::to_string(tid_iterator_register),
            };
            CONDITIONAL_JUMP = "JNEG";
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = {
                "LOAD " + std::to_string(token1->register_no),
                "STORE "+ std::to_string(tid_iterator_register),
                "LOAD "  + std::to_string(token2->register_no),
                "STORE " + std::to_string(tid_upper_limit_register),
                "SUB " + std::to_string(tid_iterator_register),
            };
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == ADDRESS && token2->type == ADDRESS) {
        if (fast_loop) {
            const auto tmp_reg = regs.add_rval();
            for_header = token1->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tmp_reg));
            for_header.splice(for_header.end(), static_cast<std::list<std::string>>(token2->translation));
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("SUB "   + std::to_string(tmp_reg));
            for_header.emplace_back("STORE "   + std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JPOS";
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = token1->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tid_iterator_register));
            for_header.splice(for_header.end(), static_cast<std::list<std::string>>(token2->translation));
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tid_upper_limit_register));
            for_header.emplace_back("SUB " + std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == ADDRESS && token2->type == LONG) {
        if (fast_loop) {
            // TODO important not to forget that simple SET cache optimization fails for this case
            // allegedly, now i'm hesitant about it
            for_header = token1->translation;
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("SUB [" + std::to_string(token2->long_value) + "]");
            for_header.emplace_back("STORE "   + std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JNEG";
            cached_constants.insert(token2->long_value);
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = token1->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tid_iterator_register));
            for_header.emplace_back("LOAD [" + std::to_string(token2->long_value) + "]");
            for_header.emplace_back("STORE " + std::to_string(tid_upper_limit_register));
            cached_constants.insert(token2->long_value);
            for_header.emplace_back("SUB " + std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == LONG && token2->type == ADDRESS) {
        if (fast_loop) {
            for_header = token2->translation;
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("SUB [" + std::to_string(token1->long_value) + "]");
            for_header.emplace_back("STORE " + std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JPOS";
            cached_constants.insert(token1->long_value);
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = token2->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tid_upper_limit_register));
            for_header.emplace_back("LOAD [" + std::to_string(token1->long_value) + "]");
            for_header.emplace_back("STORE " + std::to_string(tid_iterator_register));
            cached_constants.insert(token1->long_value);
            for_header.emplace_back("SUB " + std::to_string(tid_upper_limit_register));
            CONDITIONAL_JUMP = "JPOS";
        }
    } else if (token1->type == ADDRESS && token2->type == STRING) {
        if (fast_loop) {
            for_header = token1->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("SUB " + std::to_string(token2->register_no));
            for_header.emplace_back("STORE "+ std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JPOS";
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = token1->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tid_iterator_register));
            for_header.emplace_back("LOAD " + std::to_string(token2->register_no));
            for_header.emplace_back("STORE " + std::to_string(tid_upper_limit_register));
            for_header.emplace_back("SUB " + std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JNEG";
        }
    } else if (token1->type == STRING && token2->type == ADDRESS) {
        if (fast_loop) {
            for_header = token2->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("SUB " + std::to_string(token1->register_no));
            for_header.emplace_back("STORE "+ std::to_string(tid_iterator_register));
            CONDITIONAL_JUMP = "JNEG";
        } else {
            tid_upper_limit_register = regs.add(); // never frees the register
            for_header = token2->translation; // load address1 to r0
            for_header.emplace_back("LOADI 0");
            for_header.emplace_back("STORE " + std::to_string(tid_upper_limit_register));
            for_header.emplace_back("LOAD " + std::to_string(token1->register_no));
            for_header.emplace_back("STORE " + std::to_string(tid_iterator_register));
            // first cond check is necessary and will be added when footer is complete
            for_header.emplace_back("SUB " + std::to_string(tid_upper_limit_register));
            CONDITIONAL_JUMP = "JPOS";
        }
    }
    // conditional jump over the command block will soon be added down below!
    if (fast_loop) {
        for_footer = {
            "LOAD " + std::to_string(tid_iterator_register),
            "ADD [1]",
            CONDITIONAL_JUMP + " 3", // exit for
            "STORE " + std::to_string(tid_iterator_register),
            "JUMP -" + std::to_string(4 + loop_body_size), // jump to the first line of commands inside the loop ($8->translation)
        };
    } else {
        for_footer = {
            "LOAD " + std::to_string(tid_iterator_register),
            "ADD [1]",
            "STORE " + std::to_string(tid_iterator_register),
            "SUB " + std::to_string(tid_upper_limit_register),
            "JPOS 2", // exit for
            "JUMP -" + std::to_string(5 + loop_body_size), // jump to the first line of commands inside the loop ($8->translation)
        };
    }
    // we know exactly if the loop should be executed at least once if it is long long loop
    if (token1->type != LONG || token2->type != LONG) {
        for_header.emplace_back(CONDITIONAL_JUMP + " " + std::to_string(loop_body_size + for_footer.size() + 1));
        // jump out of the loop
    }

    cached_constants.insert(1);// we need to cache one for the footer


    return std::make_pair(for_header, for_footer);
}

void postprocess(const std::string& filename, register_table& regs) {
    std::ifstream input_file(filename);
    std::ifstream multiplication_file(math_module::multiplication_path);
    std::ifstream division_file(math_module::division_path);
    std::list<std::string> file_contents;
    std::list<std::string> multiplication_contents;
    std::list<std::string> division_contents;
    std::string line;
    // registers never yet returned by any add/assign method:
    auto last_free_reg = regs.unused_register();
    bool multiplication_flag = false; // include MUL
    bool division_flag = false;     // include DIV and MOD
    long mul_proc_line_no;
    long div_proc_line_no;
    long mul_args;
    long div_args;

    while (std::getline(input_file, line)) {
        file_contents.emplace_back(line);
        if (line.find("MUL") != std::string::npos) {
            if (multiplication_flag == false) {
                multiplication_flag = true;
                while (std::getline(multiplication_file, line)) {
                    multiplication_contents.emplace_back(line);
                }
            }
        }
        if (line.find("DIV") != std::string::npos
            || line.find("MOD") != std::string::npos
            ) {
            if (division_flag == false) {
                division_flag = true;
                while (std::getline(division_file, line)) {
                    division_contents.emplace_back(line);
                }
            }
        }
    }

    input_file.close();
    multiplication_file.close();
    division_file.close();
    std::ofstream output_file(filename);

    if (multiplication_flag) {
        mul_args = last_free_reg;
        last_free_reg = last_free_reg + 6; // NUMBER OF REGISTERS USED BY MUL
    }
    if (division_flag) {
        div_args = last_free_reg;
        last_free_reg = last_free_reg + 8; // NUMBER OF REGISTERS USED BY DIV
    }

    // calculate header offset
    // collect all cached constants
    long header_size = 0;
    std::unordered_set<long> cached_constants = {};
    for (auto& translation_line : file_contents) {
        check_for_caches(translation_line, cached_constants); // omits "this_line"'s
    }
    if (multiplication_flag) {
        for (const auto& mul_line : multiplication_contents) {
            check_for_caches(mul_line, cached_constants);
        }
    }
    if (division_flag) {
        for (const auto& div_line : division_contents) {
            check_for_caches(div_line, cached_constants);
        }
    }

    std::unordered_map<std::string, long> cache_regs = {};
    for (auto const constant : cached_constants) {
        // take care of cached constants:
        const auto cache_reg = last_free_reg;
        last_free_reg++; // use next register
        cache_regs[std::to_string(constant)] = cache_reg;

        output_file << "SET " << constant << std::endl;
        //regs.add(to_string(constant), cache_register); //???
        //output_file << "STORE [" << constant << "]" << endl; // delete
        output_file << "STORE " << cache_reg << "\t# ["<< constant << "]" << std::endl;
        header_size += 2;
    }

    int lib_size = 0;
    if (multiplication_flag) {
        mul_proc_line_no = header_size + 1; // we use one line jump to skip the lib
        lib_size += multiplication_contents.size();
    }
    if (division_flag) {
        div_proc_line_no = header_size + lib_size + 1; // we use one line jump to skip the lib
        lib_size += division_contents.size();
    }
    header_size += lib_size;

    if (multiplication_flag || division_flag) {
        output_file << "JUMP " + std::to_string(lib_size + 1) << std::endl;
        header_size += 1;
    }
    auto first_line_no_of_file_contents = header_size;
    if (file_contents.front().compare(0, 4, "JUMP") == 0) {
        header_size += 1;
    }
    // header_size fully calculated

    if (multiplication_flag) {
        for (auto& mul_line : multiplication_contents) {
            parse_math_module_line(mul_line, mul_args, cache_regs);
            output_file << mul_line << std::endl;
        }
    }
    if (division_flag) {
        for (auto& div_line : division_contents) {
            parse_math_module_line(div_line, div_args, cache_regs);
            output_file << div_line << std::endl;
        }
    }

    long line_count = first_line_no_of_file_contents;
    for (auto& translation_line : file_contents) {
        //std::cout<<translation_line<<std::endl;
        parse_proc_calls_jump(translation_line, line_count, header_size);
        line_count++;
    }

    std::list<jump_line> jump_lines;
    line_count = first_line_no_of_file_contents;
    for (auto& translation_line : file_contents) {
        check_for_jumps(translation_line, line_count, jump_lines);
        line_count++;
    }

    line_count = first_line_no_of_file_contents;
    auto it = file_contents.begin();
    while (it != file_contents.end()) {
        auto& translation_line = *it;
        auto& current_pos = it;
        auto nextIt = std::next(it);

        if (translation_line.compare(0, 3, "MUL") == 0) {
            auto translation = jump_to_mul_proc(translation_line, mul_args, mul_proc_line_no, line_count);
            update_jump_lines(jump_lines, translation.size() - 1, line_count);
            line_count += translation.size();
            file_contents.splice(current_pos, translation);
            translation_line = "#EMPTY";
        } else if (translation_line.find("DIV") != std::string::npos) {
            auto translation = jump_to_div_proc(translation_line, div_args, div_proc_line_no, line_count);
            update_jump_lines(jump_lines, translation.size() - 1, line_count);
            line_count += translation.size();
            file_contents.splice(current_pos, translation);
            translation_line = "#EMPTY";
        } else if (translation_line.find("MOD") != std::string::npos) {
            auto translation = jump_to_mod_proc(translation_line, div_args, div_proc_line_no, line_count);
            update_jump_lines(jump_lines, translation.size() - 1, line_count);
            line_count += translation.size();
            file_contents.splice(current_pos, translation);
            translation_line = "#EMPTY";
        } else if (translation_line.find("RSUBI") != std::string::npos) {
            // reverse subtraction and load value from address
            auto line_tail = translation_line.substr(6, translation_line.back());

            auto translation = {
                "STORE " + std::to_string(last_free_reg) + "\t# RSUB",
                "LOADI " + line_tail,
                "SUB " + std::to_string(last_free_reg)
            };
            last_free_reg++;
            update_jump_lines(jump_lines, translation.size() - 1, line_count);
            line_count += translation.size();
            file_contents.splice(current_pos, translation);
            translation_line = "#EMPTY";
        } else if (translation_line.find("RSUB") != std::string::npos) {
            // reverse operation
            auto line_tail = translation_line.substr(5, translation_line.back());

            auto translation = {
                "STORE " + std::to_string(last_free_reg) + "\t# RSUB",
                "LOAD " + line_tail,
                "SUB " + std::to_string(last_free_reg)
            };
            last_free_reg++;
            update_jump_lines(jump_lines, translation.size() - 1, line_count);
            line_count += translation.size();
            file_contents.splice(current_pos, translation);
            translation_line = "#EMPTY";
        } else {
            line_count++;
        }

        it = nextIt;
    }

    line_count = first_line_no_of_file_contents;
    for (auto& translation_line : file_contents) {
        if (translation_line.compare(0, 6, "#EMPTY") == 0) {
            continue;
        }
        //parse_line(translation_line, line_count, header_size, cache_regs);
        parse_proc_calls_return(translation_line, line_count);
        parse_line_cache(translation_line, cache_regs);
        output_file << translation_line << std::endl;
        line_count++;
    }
    std::cout << "file counts: " << line_count + 1 << " lines" << std::endl;
}

std::list<std::string> jump_to_mul_proc(const std::string& line, const long reg, const long dest_line_no, const long line_no) {
    if (const auto pos = line.find("MUL"); pos == std::string::npos) {
        throw std::invalid_argument("The line " + line + " is not a MUL instruction");
    }
    std::list<std::string> translation;

    if (const auto pos = line.find("MULI"); pos != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg),
            "LOADI " + line.substr(5, line.back()),
            "STORE " + std::to_string(reg + 1),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5) // shift since we added 5 lines
        };
    } else {
        translation = {
            "STORE " + std::to_string(reg),
            "LOAD " + line.substr(4, line.back()),
            "STORE " + std::to_string(reg + 1),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5) // shift since we added 5 lines
        };
    }


    return translation;
}

std::list<std::string> jump_to_div_proc(const std::string& line, const long reg, const long dest_line_no, const long line_no) {
    if (const auto pos = line.find("DIV"); pos == std::string::npos) {
        throw std::invalid_argument("The line is not a DIV instruction");
    }
    std::list<std::string> translation;

    if (line.find("RDIVI") != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg + 1),
            "LOADI " + line.substr(6, line.back()),
            "STORE " + std::to_string(reg),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5)
        };
    } else if (line.find("RDIV") != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg + 1),
            "LOAD " + line.substr(5, line.back()),
            "STORE " + std::to_string(reg),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5)
        };
    } else if (line.find("DIVI") != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg),
            "LOADI " + line.substr(5, line.back()),
            "STORE " + std::to_string(reg + 1),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5)
        };
    } else { // DIV
        // if (line.compare(0, 5, "DIV 2") == 0) {
            // translation = {"HALF"};
        // } else if (line.compare(0, 5, "DIV 4")) {
            // translation = {"HALF", "HALF"};
        // } else if (line.compare(0, 5, "DIV 8")) {
            // translation = {"HALF", "HALF", "HALF"};
        // } else {
            translation = {
                "STORE " + std::to_string(reg),
                "LOAD " + line.substr(4, line.back()),
                "STORE " + std::to_string(reg + 1),
                "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
                "STORE " + std::to_string(reg + 2),
                "JUMP " + std::to_string(dest_line_no - line_no - 5)
            };
        // }
    }
    return translation;
}

std::list<std::string> jump_to_mod_proc(const std::string& line, const long reg, const long dest_line_no, const long line_no) {
    if (const auto pos = line.find("MOD"); pos == std::string::npos) {
        throw std::invalid_argument("The line is not a MOD instruction");
    }
    std::list<std::string> translation;

    if (line.find("RMODI") != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg + 1),
            "LOADI " + line.substr(6, line.back()),
            "STORE " + std::to_string(reg),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5),
            "LOAD " + std::to_string(reg), // here is the remainder stored we need to get here after returning from proc
        };
    } else if (line.find("RMOD") != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg + 1),
            "LOAD " + line.substr(5, line.back()),
            "STORE " + std::to_string(reg),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5),
            "LOAD " + std::to_string(reg), // here is the remainder stored we need to get here after returning from proc
        };
    } else if (line.find("MODI") != std::string::npos) {
        translation = {
            "STORE " + std::to_string(reg),
            "LOADI " + line.substr(5, line.back()),
            "STORE " + std::to_string(reg + 1),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5),
            "LOAD " + std::to_string(reg), // here is the remainder stored we need to get here after returning from proc
        };
    } else { // regular MOD
        translation = {
            "STORE " + std::to_string(reg),
            "LOAD " + line.substr(4, line.back()),
            "STORE " + std::to_string(reg + 1),
            "SET " + std::to_string(line_no + 6), // bigger shift since line_no points to the translation.front()
            "STORE " + std::to_string(reg + 2),
            "JUMP " + std::to_string(dest_line_no - line_no - 5),
            "LOAD " + std::to_string(reg), // here is the remainder stored we need to get here after returning from proc
        };
    }
    return translation;
}

void check_for_jumps(std::string& line, const long line_no, std::list<jump_line>& jump_lines) {
    if (line.compare(0, 1, "J") == 0) {
        jump_lines.emplace_back(line, line_no);
    }
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
        // std::cout << "cached value: " << cached_value << std::endl;
    }
}

void parse_math_module_line(std::string& line, const long arg_reg, const std::unordered_map<std::string, long> &cache_regs) {
    if (const auto pos = line.find("p+"); pos != std::string::npos) {
        line.replace(pos, 3, std::to_string(arg_reg + std::stol(line.substr(pos + 2, 1))));
    }
    if (const auto pos = line.find("p"); pos != std::string::npos) {
        line.replace(pos, 1, std::to_string(arg_reg));
    }

    if (const auto pos = line.find("["); pos != std::string::npos) {
        if (const auto comment_pos = line.find("#"); comment_pos != std::string::npos && comment_pos < pos) {
            return; // it's a comment
        }
        const auto end_pos = line.find("]", pos);
        const auto val_length = end_pos - pos - 1;
        const auto& cached_value = line.substr(pos + 1, val_length);
        line.replace(pos, val_length + 2, std::to_string(cache_regs.at(cached_value)) + "\t# [" + cached_value + "]");
    }

}

void parse_line_cache(std::string& line, const std::unordered_map<std::string, long>& cache_regs) {
    // this function assumes that all cached constants are already cached

    //replace [cached] value with its reg
    if (const auto pos = line.find("["); pos != std::string::npos) {
        if (const auto comment_pos = line.find("#"); comment_pos != std::string::npos && comment_pos < pos) {
            return; // it's a comment
        }
        const auto end_pos = line.find("]", pos);
        const auto val_length = end_pos - pos - 1;
        const auto& cached_value = line.substr(pos + 1, val_length);
        //std::cout << "line: " << line << std::endl;
        //std::cout << "cached value: " << cached_value << std::endl;
        line.replace(pos, val_length + 2, std::to_string(cache_regs.at(cached_value)) + "\t# [" + cached_value + "]");
        // TODO delete this line:
        //line.append("\t# [" + cached_value + "] " + std::to_string(std::count(line.begin(), line.end(), '\n')));
    }
}

void parse_proc_calls_return(std::string& line, const long line_no) {

    //replace this_line with actual line_no
    if (const auto pos = line.find("this_line"); pos != std::string::npos) {
        if (const auto pos_proc_call_load_line = line.find("[this_line + 3]"); pos_proc_call_load_line != std::string::npos) {
            line.replace(pos_proc_call_load_line, 15, std::to_string(line_no + 3));
        }
    }
}

void parse_proc_calls_jump(std::string& line, const long line_no, const long header_offset) {
    //replace this_line with actual line_no
    if (const auto pos = line.find("this_line"); pos != std::string::npos) {
        if (const auto pos_proc_call_jump_to_proc = line.find("JUMP "); pos_proc_call_jump_to_proc != std::string::npos) {
            const int value_length = line.find(" -") - line.front();
            const auto value = std::stoi(line.substr(pos_proc_call_jump_to_proc + 4, value_length));
            line = "JUMP " + std::to_string(value - line_no + header_offset)
            + "\t# offset: +" + std::to_string(header_offset)
            + "\t# original: " + std::to_string(value) + " - " + std::to_string(line_no);
        }
    }
}

// probably the dumbest thing I've had to resort to in order to avoid jump operations breaking
// this function is ment to be invoked in parsing proc_call's lines
void amend_for_additional_lines(std::list<std::string>& proc_commands, const long num_of_additional_lines, const long line_no) {
    // if the proc_commands end up bigger after parsing references
    // this function makes up for the offset in each jump that was including the line
    // std::list<std::pair<std::_List_iterator<std::string>, long>> jump_calls;
    std::list<std::pair<std::list<std::string>::iterator, long>> jump_forward_calls;
    auto line_count = 0;

    // iterate to push forward jumps
    for (auto it = proc_commands.begin(); it != proc_commands.end(); ++it, ++line_count) {
        auto& line = *it;

        if (line_count == line_no) {
            for (auto call : jump_forward_calls) {
                auto& call_str = *call.first;
                const auto argument_pos = call_str.find(" ") + 1;
                auto argument = std::stol(call_str.substr(argument_pos));
                argument = argument + num_of_additional_lines;
                call_str.replace(argument_pos, std::string::npos, std::to_string(argument));
            }

            break;
        }

        if (line.compare(0, 1, "J") == 0) {
            const auto argument_pos = line.find(" ") + 1;

            //if (line_count < line_no) {
            if (auto argument = std::stol(line.substr(argument_pos)); argument > 0) {
                auto jump_call = std::make_pair(it, argument);
                jump_forward_calls.push_back(jump_call);
            }
            //}
        }

        auto jump_iterator = jump_forward_calls.begin();
        while (jump_iterator != jump_forward_calls.end()) {
            auto& jump = *jump_iterator;
            auto& str = jump.first;
            auto& length = jump.second;
            length = length - 1;
            length = length - std::ranges::count(line, '\n');
            if (length <= 0) {
                jump_iterator = jump_forward_calls.erase(jump_iterator);
            } else {
                ++jump_iterator;
            }
        }

        line_count += std::ranges::count(line, '\n');;
    }

    // iterate backwards to push backward jumps
    std::list<std::pair<std::list<std::string>::reverse_iterator, long>> jump_backward_calls;
    line_count = proc_commands.size() - 1;
    for (auto it = proc_commands.rbegin(); it != proc_commands.rend(); ++it, --line_count) {
        auto& line = *it;

        if (line_count == line_no) {
            for (auto call : jump_backward_calls) {
                auto& call_str = *call.first;
                const auto argument_pos = call_str.find(" ") + 1;
                auto argument = std::stol(call_str.substr(argument_pos));
                argument = argument - num_of_additional_lines;
                std::cout << "BEFORE JUMP: " << call_str << std::endl;
                call_str.replace(argument_pos, std::string::npos, std::to_string(argument));
                std::cout << "repaired JUMP: " << call_str << std::endl;
            }
            break;
        }

        if (line.compare(0, 1, "J") == 0) {
            const auto argument_pos = line.find(" ") + 1;

            //if (line_count < line_no) {
            if (auto argument = std::stol(line.substr(argument_pos)); argument < 0) {
                auto jump_call = std::make_pair(it, argument);
                jump_backward_calls.push_back(jump_call);
                std::cout << "FOUND JUMP: " << line << std::endl;
            }
            //}
        }

        auto jump_iterator = jump_backward_calls.begin();
        while (jump_iterator != jump_backward_calls.end()) {
            auto& jump = *jump_iterator;
            auto& length = jump.second;
            length = length + 1;
            length = length + std::ranges::count(line, '\n');
            if (length > 0) {
                jump_iterator = jump_backward_calls.erase(jump_iterator);
            } else {
                ++jump_iterator;
            }
        }

        line_count -= std::ranges::count(line, '\n');
    }

}

void parse_proc_line(std::string& line, std::list<std::string>& proc_commands, const std::list<long>& arg_regs, const long line_no) {
    const auto line_comment = line.find("\t#") == std::string::npos ? "" : line.substr(line.find("\t#"));
    for (const auto& reg : arg_regs) {
        const auto& reg_str = std::to_string(reg);
        if (const auto pos = line.find(reg_str); pos != std::string::npos) {
            auto stripped_line = line.find("\t#") == std::string::npos ? line : line.substr(0, line.find("\t#"));
            stripped_line = stripped_line.find(" #") == std::string::npos ? stripped_line : stripped_line.substr(0, stripped_line.find(" #")) ;
            // std::cout << "line: \""<< line << '"' << std::endl;
            // std::cout << "stripped line: \""<< stripped_line << '"' << std::endl;
            if (stripped_line.find(reg_str) == std::string::npos) {
                //std::cout << "FALSE POSITIVE: "<< line << ";\tregister: " << reg_str << std::endl;
                continue; // false positive
            }

            std::string argument;
            if (stripped_line.find("[") != std::string::npos) {
                argument = stripped_line.substr(stripped_line.find(" ") + 2);
            } else {
                argument = stripped_line.substr(stripped_line.find(" ") + 1);
            }
            // std::cout << "argument: \""<< argument  << '"' << std::endl;
            // std::cout << "real argument: \""<< reg_str << '"' << std::endl;
            if (argument.compare(reg_str) != 0 && argument.compare(reg_str + "]") != 0) {
                //std::cout << "FALSE POSITIVE: "<< line << ";\tregister: " << reg_str << std::endl;
                continue; // false positive
            }


            if (line.compare(0, 4, "LOAD") == 0) {
                if (line.find("# passing address") != std::string::npos) {
                    // "passing address" present then two cases:
                    // if it was reference pass we pass it as is (do nothing)
                    // if it was a proc call then erase brackets
                    const auto close_bracket_pos = line.find(']');
                    const auto open_bracket_pos = line.find('[');
                    if (close_bracket_pos != std::string::npos && open_bracket_pos != std::string::npos) {
                        line.erase(close_bracket_pos, 1);
                        line.erase(open_bracket_pos, 1);
                    }
                } else {
                    if (line.find("[") != std::string::npos) {
                        return; // the arg was used as a constant
                    }
                    line = "LOADI " + reg_str + line_comment;
                }

                continue;
            }

            // LOAD was the only case that could interfere with [ ] constants
            if (line.find("[") != std::string::npos) {
                return; // the arg was used as a constant
            }
            // TODO replace OP by OPI with find() to leave optional ROP intact
            if (line.compare(0, 5, "LOADI") == 0) {
                line = "LOADI " + reg_str + "\nLOADI 0";
                amend_for_additional_lines(proc_commands, 1, line_no);
                std::cout << " LINE : " << line << ", lineno: " << line_no << " needed to be repaired" << std::endl;
            } else if (line.compare(0, 5, "STORE") == 0) {
                line = "STOREI " + reg_str;
            } else if (line.compare(0, 6, "STOREI") == 0) {
                line = "STOREII " + reg_str;
            } else if (line.compare(0, 3, "ADD") == 0) {
                line = "ADDI " + reg_str;
            } else if (line.compare(0, 4, "RADD") == 0) {
                line = "RADDI " + reg_str;
            } else if (line.compare(0, 4, "ADDI") == 0) {
                //line = "ADDII " + reg_str;
            } else if (line.compare(0, 3, "SUB") == 0) {
                line = "SUBI " + reg_str;
            } else if (line.compare(0, 4, "SUBI") == 0) {
                //line = "SUBII " + reg_str;
            } else if (line.compare(0, 4, "RSUB") == 0) {
                line = "RSUBI " + reg_str;
            } else if (line.compare(0, 3, "MUL") == 0) {
                line = "MULI " + reg_str;
            } else if (line.compare(0, 3, "DIV") == 0) {
                line = "DIVI " + reg_str;
            } else if (line.compare(0, 4, "RDIV") == 0) {
                line = "RDIVI " + reg_str;
            } else if (line.compare(0, 3, "MOD") == 0) {
                line = "MODI " + reg_str;
            } else if (line.compare(0, 4, "RMOD") == 0) {
                line = "RMODI " + reg_str;
            } else if (line.compare(0, 3, "GET") == 0) {
                line = "GET 0\nSTOREI " + reg_str;
                amend_for_additional_lines(proc_commands, 1, line_no);
                std::cout << " LINE : " << line << ", lineno: " << line_no << " needed to be repaired" << std::endl;
            } else if (line.compare(0, 3, "PUT") == 0) {
                line = "LOADI " + reg_str + "\nPUT 0";
                amend_for_additional_lines(proc_commands, 1, line_no);
                std::cout << " LINE : " << line << ", lineno: " << line_no << " needed to be repaired" << std::endl;
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

void register_table::set_initialized(const std::string& pid) {
    table.at(pid).initialized = true;
}

void register_table::check_if_initialized(const std::string& pid) const {
    if (!table.at(pid).initialized) {
        throw std::runtime_error("WARNING: UNINITIALIZED VARIABLE: " + pid);
    }
}

int register_table::assign_registers(const int size) {
    if (free_registers.empty()) {
        throw std::runtime_error("MEMORY ERROR: OUT OF REGISTERS");
    }
    //std::cout << "assignment size: "<<size<<std::endl<<"free regs before oparation:" << std::endl;
    //for (const auto& [first, last] : free_registers) {
    //    printf("from %lld to %lld\n", first, last);
    //}

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
    const auto range_end = it->second;
    free_registers.erase(it);
    if (range_end - reg > size) {
        const auto chopped_range = std::make_pair(reg + size, range_end);
        free_registers.insert(chopped_range);
    }


    // last register is the first unused by the assigment [first, last)
    first_untouched_register = std::max(first_untouched_register, static_cast<long>(free_registers.begin()->first));
    first_untouched_register = std::max(first_untouched_register, static_cast<long>(reg + size));


    // std::cout << "free regs after oparation:" << std::endl;
    // for (const auto& [first, last] : free_registers) {
        // printf("from %lld to %lld\n", first, last);
    // }
    // std::cout << "first_untouched_reg" << first_untouched_register << std::endl;
    return reg;
}

register_table::register_table() : free_registers(), table() {
    free_registers.insert(std::make_pair(1,2<<29));
    // r0 is used as a special temporary register
}

void register_table::remove(const std::string& pid) { // maybe free_pid() ?
    auto [size, reg, index_shift, initialized] = table.at(pid);
    int from = reg;
    int to = from + size;
    auto range = std::make_pair(from, to);

    // TODO uncomment (and find a bug)
    // auto lower_bound = free_registers.lower_bound(range);
    // lower_bound = (lower_bound == free_registers.begin())? lower_bound : --lower_bound;
    // const auto upper_bound = free_registers.upper_bound(range);
    // //std::cout << "OOUOUOUOUO : Lb.1: " << lower_bound->first << ", Lb.2: " << lower_bound->second <<", Ub.1: " << upper_bound->first <<", Ub.2: " <<upper_bound->second << std::endl;
    // if (lower_bound->second < reg || reg < upper_bound->first - 1) {
    //     free_registers.insert(range);
    // } else if (lower_bound->second + 1 == upper_bound->first) {
    //     range.first = lower_bound->first;
    //     range.second = upper_bound->second;
    //     free_registers.erase(lower_bound);
    //     free_registers.erase(upper_bound);
    //     free_registers.insert(range);
    // } else if (lower_bound->second == reg) {
    //     range.first = lower_bound->first;
    //     free_registers.erase(lower_bound);
    //     free_registers.insert(range);
    // } else if (upper_bound->first - 1 == reg) {
    //     range.second = upper_bound->first;
    //     free_registers.erase(upper_bound);
    //     free_registers.insert(range);
    // }

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

    const auto reg = assign_register();
    return reg;
    // deprecated (or find the bug):

    // const auto it = free_registers.begin();
    // const auto reg = it->first;
    //
    // first_untouched_register = std::max(first_untouched_register, static_cast<long>(reg + 1));
    // return reg;
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
        // throw std::runtime_error("Error: Redeclaration of variable");
        return table[pid].register_no;
    } //TODO it was simply commented out you need to handle it in the parser

    pid_type new_pid;
    new_pid.size = to - from + 1;
    if (new_pid.size == 1) {
        throw std::invalid_argument("this program won't allow tables with size of 1! Use a variable instead.");
    }
    if (new_pid.size <= 0) {
        throw std::invalid_argument("this program cannot declare insideout tables!");
    }
    new_pid.index_shift = from;
    new_pid.register_no = assign_registers(new_pid.size);

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


// jump_lines
jump_line::jump_line(std::string& line_reference, const long line_number): line_no(line_number), command_line(line_reference) {

    // get the comment
    const auto comment_pos = line_reference.find("\t#");
    if (comment_pos != std::string::npos) {
        comment = line_reference.substr(comment_pos);
    } else {
        comment = "";
    }

    // get the opcode
    opcode = line_reference.substr(0 , line_reference.find(" "));

    // get the argument
    const auto stripped_line = comment.empty() ? line_reference : line_reference.substr(0, line_reference.find("\t#"));
    if (opcode.compare(0, 1, "J") == 0) {
        const auto argument_pos = line_reference.find(" ") + 1;
        argument = std::stol(stripped_line.substr(argument_pos));
    } else {
        throw std::invalid_argument("the line" + line_reference + " seems not to be a jump call");
    }
}
void jump_line::update_line_no(const long new_line_no) {
    line_no = new_line_no;
}

// assuming position of the additional lines, updates the jump so the jump remains consistent with usual program flow
void jump_line::update_argument(const long additional_lines_no, const long position) {
    const auto dest_line_no = argument + line_no;
    if (argument > 0) { // forward jump
        if (line_no < position && position < dest_line_no) {
            // update argument
            argument += additional_lines_no;
            command_line = opcode + " " + std::to_string(argument) + comment;
        }
    } else { // backward jump
        if (dest_line_no <= position && position < line_no) {
            // update argument
            argument -= additional_lines_no;
            command_line = opcode + " " + std::to_string(argument) + comment;
        }
    }

    if (position < line_no) {
        line_no += additional_lines_no;
    }
}


void update_jump_lines(std::list<jump_line>& jump_lines, const long additional_lines_no, const long position) {
    for (auto& jump : jump_lines) {
        jump.update_argument(additional_lines_no, position);
    }
}

//MISCELLANEOUS
std::string extract_function_name(const std::string& fun_call) {
    const size_t pos = fun_call.find('(');
    if (pos == std::string::npos) {
        throw std::invalid_argument("wrong number of arguments! \nin function call: " + fun_call);
    }
    return fun_call.substr(0, pos);
}


