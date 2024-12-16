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
#include "parser.h"

// struct PairHash {
//     template <typename T1, typename T2>
//     std::size_t operator()(const std::pair<T1, T2>& pair) const {
//         std::size_t h1 = std::hash<T1>()(pair.first);
//         std::size_t h2 = std::hash<T2>()(pair.second);
//         return h1 ^ (h2 << 1); // Combine the two hashes
//     }
// };

TokenAttribute* parse_condition(TokenAttribute*, TokenAttribute*, std::list<std::string>, const std::list<std::string>&, const bool, const int);

struct CompareFirstPairEntry {
    bool operator()(const std::pair<long long, long long>& a, const std::pair<long long, long long>& b) const {
        return a.first == b.first? a.second < b.second : a.first < b.first;
    }
};

struct pid_type {
    //auto type = INTEGER (this is a toy compiler)
    int size;
    int register_no; //first reg occupied by this pid
    int index_shift = 0; // first value from the table range [first, second]
    int at(const int index) const {
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

public:
    register_table();

    void remove(const std::string &pid);

    void remove(int reg);

    int at(const std::string &pid) const;

    int at(const std::string &pid, int index) const;

    int add_rval() const;

    int contains(const std::string&) const;

    int add(const std::string &pid);

    int add();

    int add_table(const std::string &pid, int from, int to);

    int assign_register(void);

    int assign_registers(int size);
};



#endif //PARSER_UTILS_H
