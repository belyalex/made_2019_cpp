#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <cstdlib>

class LinearAllocator
{
public:
    LinearAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
private:
    char* buf=nullptr;
    char* pos=nullptr;
    size_t max_size=0;
public:
    ~LinearAllocator() {if (buf!=nullptr) free(buf);};
};


#endif
