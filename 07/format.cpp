//
// Created by alex on 28.11.2019.
//
#include "format.h"

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

std::string collect_params(const std::string &fmt, std::vector<std::string> &strings) {
    return subst(fmt, strings);
}

std::string format(const std::string &fmt) {
    std::vector<std::string> strings;
    return subst(fmt, strings);
}


