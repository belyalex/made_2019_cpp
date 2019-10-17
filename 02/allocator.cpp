#include "allocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) {
    if (maxSize > 0) {
        max_size = maxSize;
        buf = (char*)malloc(maxSize);
        pos = buf;
    }
} 

char* LinearAllocator::alloc(size_t size) {
    if ((buf != nullptr) && (size > 0) && (max_size-(pos - buf) >= size)) {
	char* tmp = pos;
	pos += size;
        return tmp;
    }
    return nullptr;
}

void LinearAllocator::reset() {
    pos = buf;
}
