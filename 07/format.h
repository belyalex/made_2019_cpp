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

std::string subst(const std::string &fmt, const std::vector<std::string> &strings);

std::string collect_params(const std::string &fmt, std::vector<std::string> &strings);

template<typename Arg, typename ... Args>
std::string collect_params(const std::string &fmt, std::vector<std::string> &strings, Arg &&arg, Args &&... args) {
    strings.push_back(to_string(std::forward<Arg>(arg)));
    return collect_params(fmt, strings, std::forward<Args>(args) ...);
}

std::string format(const std::string &fmt);

template<typename ... Args>
std::string format(const std::string &fmt, Args &&... args) {
    std::vector<std::string> strings;
    return collect_params(fmt, strings, std::forward<Args>(args) ...);
}

#endif //FORMAT_H
