//
// Created by adame on 12/8/2024.
//

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H
#include <list>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "parser.h"

// struct PairHash {
//     template <typename T1, typename T2>
//     std::size_t operator()(const std::pair<T1, T2>& pair) const {
//         std::size_t h1 = std::hash<T1>()(pair.first);
//         std::size_t h2 = std::hash<T2>()(pair.second);
//         return h1 ^ (h2 << 1); // Combine the two hashes
//     }
// };

struct math_module {
public:
    static const std::string multiplication_path;
    static const std::string division_path;
};

struct pid_type;
class register_table;
TokenAttribute* parse_expression(TokenAttribute*, TokenAttribute*, const std::string&, const std::string&, long, long);
TokenAttribute* parse_condition(TokenAttribute*, TokenAttribute*, std::list<std::string>, const std::list<std::string>&, bool, int, int, std::unordered_set<long>&);
std::pair<std::list<std::string>, std::list<std::string>> parse_for_loop(const TokenAttribute*, const TokenAttribute*, long, long, std::unordered_set<long>&, register_table&, const bool);
void check_for_caches(const std::string&, std::unordered_set<long>&);
void parse_line(std::string&, long, long, std::unordered_map<std::string, long>&);
void parse_proc_line(std::string&, std::list<std::string>&, const std::list<long>&, long);
void parse_math_module_line(std::string&, long, const std::unordered_map<std::string, long> &);
void postprocess(const std::string&, register_table&);
std::list<std::string> jump_to_mul_proc(const std::string&, long, long, long);
std::list<std::string> jump_to_div_proc(const std::string&, long, long, long);
std::list<std::string> jump_to_mod_proc(const std::string&, long, long, long);


struct CompareFirstPairEntry {
    bool operator()(const std::pair<long long, long long>& a, const std::pair<long long, long long>& b) const {
        return a.first == b.first? a.second < b.second : a.first < b.first;
    }
};

class funs_table {
private:
    std::unordered_map<std::string, long> line_no_of_; // (fun_name, line_no)
    std::unordered_map<std::string, long> arg_count_of_;
    std::unordered_map<std::string, long> first_arg_register_of_;

public:
    funs_table() = default;

    void add(const std::string &fun_name, long line_no, long amount_of_arguments, long first_reg);
    long get_line_no(const std::string& fun_name) const;
    long get_arg_count(const std::string& fun_name) const;

    long get_first_arg_register(const std::string &fun_name) const;
};

struct pid_type {
    //auto type = INTEGER (this is a toy compiler)
    int size;
    int register_no; //first reg occupied by this pid
    int index_shift = 0; // first value from the table range [first, second]
    int at(const int index) const {
        if (size == 0) {
            return register_no + index;
            throw std::runtime_error("Array type access to a scalar variable is not allowed"
                                     "\nInternal error: cannot ask for table known only at runtime");
        }
        const auto offset = index - index_shift;
        if (offset < 0 || offset >= size) {
            throw std::runtime_error("Access to elements outside the table is not allowed");
        }
        return register_no + offset;
    }
};

class register_table {
private:
    std::set<std::pair<long long int, long long int>, CompareFirstPairEntry> free_registers; // (interval)
    std::unordered_map<std::string, pid_type> table; // (pid, (size,register))
    long first_untouched_register; // lowest register that was never used by any pid or tid

public:
    register_table();

    void remove(const std::string &pid);

    void remove(int reg);

    void forget_pid(const std::string &pid);

    int at(const std::string &pid) const;

    int at(const std::string &pid, int index) const;

    int add_rval();

    pid_type get_pid(const std::string &pid) const;

    int contains(const std::string&) const;

    int add(const std::string &pid);

    int add();

    void add_cache_reg(const std::string &pid, long long reg);

    int add_table(const std::string &pid, int from, int to);

    int add_proc_table(const std::string &pid);

    long unused_register() const;

    int assign_register(void);

    int assign_registers(int size);
};



#endif //PARSER_UTILS_H
