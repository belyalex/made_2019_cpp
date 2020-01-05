//
// Created by alex on 04.01.2020.
//

#ifndef CPP_TASK08_ITERATOR_H
#define CPP_TASK08_ITERATOR_H

#include <iterator>

template<class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
    T *ptr;
public:
    explicit Iterator(T *ptr) : ptr(ptr) {}

    bool operator==(const Iterator<T> &other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const Iterator<T> &other) const {
        return ptr != other.ptr;
    }

    bool operator<(const Iterator<T> &other) const {
        return ptr < other.ptr;
    }

    bool operator>=(const Iterator<T> &other) const {
        return !(*this < other);
    }

    bool operator>(const Iterator<T> &other) const {
        return other < *this;
    }

    bool operator<=(const Iterator<T> &other) const {
        return !(other < *this);
    }

    T &operator*() const {
        return *ptr;
    }

    Iterator<T> &operator++() {
        ++ptr;
        return *this;
    }

    Iterator<T> operator++(int) {
        Iterator<T> tmp(*this);
        ptr++;
        return tmp;
    }

    Iterator<T> &operator--() {
        --ptr;
        return *this;
    }

    Iterator<T> operator--(int) {
        Iterator tmp(*this);
        ptr--;
        return tmp;
    }

    Iterator<T> operator+(int add) const {
        Iterator tmp(*this);
        tmp.ptr += add;
        return tmp;
    }

    Iterator<T> operator-(int sub) const {
        Iterator tmp(*this);
        tmp.ptr -= sub;
        return tmp;
    }

    Iterator<T> &operator+=(int add) {
        ptr += add;
        return *this;
    }

    Iterator<T> &operator-=(int sub) {
        ptr -= sub;
        return *this;
    }
};


#endif //CPP_TASK08_ITERATOR_H
