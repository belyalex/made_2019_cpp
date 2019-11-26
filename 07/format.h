//
// Created by alex on 26.11.2019.
//

#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <sstream>
#include <vector>


template<typename T>
std::string to_string(T&& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

std::string subst(const std::string &fmt, const std::vector<std::string> &strings) {
    std::string res;
    size_t i = 0;
    while (i < fmt.size()) {
        if (fmt[i] == '{') {
            i++;
            if ((fmt[i]<'0')||(fmt[i]>'9')) {
                throw std::runtime_error("Expected number in {} placeholder.");
            }
            size_t n = 0;
            while ((fmt[i] >= '0') && (fmt[i] <= '9')) {
                n = n * 10 + fmt[i] - '0';
                i++;
            }
            if (fmt[i] != '}') {
                throw std::runtime_error("Expected '}'.");
            }
            if (n>=strings.size()) {
                throw std::runtime_error("Placeholder number is too much.");
            }
            res+=strings[n];
            i++;
            continue;
        }
        if (fmt[i] == '}') {
            throw std::runtime_error("Unexpected '}'.");
        }
        res+=fmt[i];
        i++;
    }
    return res;
}

template<typename Arg, typename ... Args>
std::string subst(const std::string &fmt, std::vector<std::string> &strings, Arg &&arg, Args &&... args) {
    strings.push_back(to_string(std::forward<Arg>(arg)));
    return subst(fmt, strings, std::forward<Args>(args) ...);
}

std::string format(const std::string &fmt) {
    return fmt;
}

template<typename ... Args>
std::string format(const std::string &fmt, Args &&... args) {
    std::vector<std::string> strings;
    return subst(fmt, strings, std::forward<Args>(args) ...);
}


#endif //FORMAT_H
