//
// Created by alex on 04.01.2020.
//

#ifndef CPP_TASK08_VECTOR_H
#define CPP_TASK08_VECTOR_H

#include <iterator>
#include <cassert>
#include "Iterator.h"

const size_t INITIAL_CAPACITY = 16;

template<class T, class Allocator=std::allocator<T>>
class Vector {
public:
    Vector() : size_(0), capacity_(INITIAL_CAPACITY), buffer(allocator.allocate(INITIAL_CAPACITY)) {}

    explicit Vector(size_t size, const T &default_value = T()) : size_(size), capacity_(size),
                                                                 buffer(allocator.allocate(size)) {
        for (size_t i = 0; i < size; i++) {
            allocator.construct(&buffer[i], default_value);
        }
    }

    Vector(std::initializer_list<T> list) {
        size_ = capacity_ = list.end() - list.begin();
        buffer = allocator.allocate(size_);
        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); it++) {
            allocator.construct(&buffer[i++], *it);
        }
    }

    explicit Vector(const Vector<T> &copied) : size_(copied.size_), capacity_(copied.capacity_),
                                               buffer(allocator.allocate(copied.size_)) {
        for (size_t i = 0; i < size_; i++) {
            allocator.construct(&buffer[i], copied[i]);
        }
    }

    explicit Vector(Vector<T> &&moved) : size_(moved.size_), capacity_(moved.capacity_), buffer(moved.buffer) {
        moved.buffer = nullptr;
        moved.capacity_ = 0;
        moved.size_ = 0;
    }

    ~Vector() {
        clear();
        allocator.deallocate(buffer, capacity_);
    }

    Vector<T> &operator=(const Vector<T> &copied) {
        if (&copied == this) {
            return *this;
        }
        auto tmp = allocator.allocate(copied.size_);
        clear();
        allocator.deallocate(buffer, capacity_);
        buffer = tmp;
        size_ = copied.size_;
        capacity_ = copied.capacity_;
        for (size_t i = 0; i < size_; i++) {
            allocator.construct(&buffer[i], copied.buffer[i]);
        }
        return *this;
    }

    Vector<T> &operator=(Vector<T> &&moved) {
        if (&moved == this) {
            return *this;
        }
        clear();
        allocator.deallocate(buffer, capacity_);
        buffer = moved.buffer;
        size_ = moved.size_;
        capacity_ = moved.capacity_;
        moved.buffer = nullptr;
        moved.size_ = 0;
        moved.capacity_ = 0;
    }

    T &operator[](size_t index) {
        assert(index < size_);
        return buffer[index];
    }

    const T &operator[](size_t index) const {
        assert(index < size_);
        return buffer[index];
    }

    void push_back(const T &element) {
        if (size_ == capacity_) {
            realloc(capacity_ * 2);
        }
        allocator.construct(&buffer[size_++], element);
    }

    void pop_back() {
        allocator.destroy(&buffer[--size_]);
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    void clear() {
        for (size_t i = 0; i < size_; i++) {
            allocator.destroy(&buffer[i]);
        }
    }

    Iterator<T> begin() {
        return Iterator<T>(buffer);
    }

    Iterator<T> end() {
        return Iterator<T>(buffer + size_);
    }

    std::reverse_iterator<Iterator<T>> rbegin() {
        return std::reverse_iterator<Iterator<T>>(end());
    }

    std::reverse_iterator<Iterator<T>> rend() {
        return std::reverse_iterator<Iterator<T>>(begin());
    }

    void resize(size_t new_size, const T &default_value = T()) {
        if (size_ != new_size) {
            if (new_size > capacity_) {
                auto tmp = allocator.allocate(new_size);
                for (size_t i = 0; i < new_size; i++) {
                    if (i < size_) {
                        allocator.construct(&tmp[i], buffer[i]);
                    } else {
                        allocator.construct(&tmp[i], default_value);
                    }
                }
                clear();
                allocator.deallocate(buffer, capacity_);
                buffer = tmp;
                capacity_ = new_size;
                size_ = new_size;
            } else {
                if (size_ < new_size) {
                    for (size_t i = size_; i < new_size; i++) {
                        allocator.construct(&buffer[i], default_value);
                    }
                } else {
                    for (size_t i = new_size; i < size_; i++) {
                        allocator.destroy(&buffer[i]);
                    }
                }
                size_ = new_size;
            }
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            realloc(new_capacity);
        }
    }

private:
    size_t size_;
    size_t capacity_;
    T *buffer;
    Allocator allocator;

    void realloc(size_t new_capacity, const T &default_value = T()) {
        auto tmp = allocator.allocate(new_capacity);
        size_t new_size = std::min(size_, new_capacity);
        for (size_t i = 0; i < new_size; i++) {
            if (i < size_) {
                allocator.construct(&tmp[i], buffer[i]);
            } else {
                allocator.construct(&tmp[i], default_value);
            }
        }
        clear();
        allocator.deallocate(buffer, capacity_);
        buffer = tmp;
        capacity_ = new_capacity;
        size_ = new_size;
    }
};

#endif //CPP_TASK08_VECTOR_H
