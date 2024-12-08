//
// Created by adame on 12/8/2024.
//

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H
#include <string>
#include <unordered_map>
#include <unordered_set>

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);
        return h1 ^ (h2 << 1); // Combine the two hashes
    }
};

struct pid_type {
    //auto type = INTEGER (this is a toy compiler)
    int size;
    int register_no; //first reg occupied by this pid
};

class register_table {
private:
    std::unordered_set<std::pair<int, int>, PairHash> free_registers; // (interval)
    std::unordered_map<std::string, pid_type> table; // (pid, (size,register))

public:
    register_table();

    void remove(const std::string &pid);

    int at(const std::string &pid) const;

    int add_rval() const;

    int add(const std::string &pid);

    void add_table(const std::string &pid, int from, int to);

    int available_register(void);
};


#endif //PARSER_UTILS_H
