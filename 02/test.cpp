#include <iostream>
#include "allocator.h"

#define TESTS_BEGIN(c) \
	int N_TESTS = 0; \
	int N_FAILED_TESTS = 0; \
	c; \
	std::cout << "Tests " << #c << " started." << std::endl; 

#define TEST(c)  do { \
    N_TESTS++; \
    if (!(c)) { \
	N_FAILED_TESTS++; \
        std::cerr << "Test " << #c << " failed in " << __FILE__ \
                  << "(line " << __LINE__ << ") : " << std::endl; \
	} \
	else { \
		std::cout << "Test " << #c << " done successfully" << std::endl; \
	} \
} while(0);

#define TESTS_END \
	std::cout << "Failed " << N_FAILED_TESTS << " from " << N_TESTS << " tests." << std::endl;

void test0() {
    TESTS_BEGIN(LinearAllocator a(0))
    TEST(a.alloc(0)==nullptr)
    TEST(a.alloc(1)==nullptr)
    TEST(a.alloc(-1)==nullptr)
    TESTS_END
}

void test1() {
    TESTS_BEGIN(LinearAllocator a(1))
    TEST(a.alloc(0)==nullptr)
    TEST(a.alloc(1)!=nullptr)
    TEST(a.alloc(1)==nullptr)
    TEST(a.alloc(-1)==nullptr)
    TESTS_END
}

void test10() {
    TESTS_BEGIN(LinearAllocator a(10))
    TEST(a.alloc(0)==nullptr)
    TEST(a.alloc(1)!=nullptr)
    TEST(a.alloc(1)!=nullptr)
    TEST(a.alloc(-1)==nullptr)
    TEST(a.alloc(8)!=nullptr)
    TEST(a.alloc(1)==nullptr)
    TESTS_END
}

void test10_reset() {
    TESTS_BEGIN(LinearAllocator a(10))
    TEST(a.alloc(0)==nullptr)
    TEST(a.alloc(1)!=nullptr)
    TEST(a.alloc(1)!=nullptr)
    TEST(a.alloc(-1)==nullptr)
    TEST(a.alloc(8)!=nullptr)
    TEST(a.alloc(1)==nullptr)
    a.reset();
    TEST(a.alloc(0)==nullptr)
    TEST(a.alloc(4)!=nullptr)
    TEST(a.alloc(5)!=nullptr)
    TEST(a.alloc(-1)==nullptr)
    TEST(a.alloc(1)!=nullptr)
    TEST(a.alloc(1)==nullptr)
    TESTS_END
}

void test_all() {
    test0();

    test1();

    test10();

    test10_reset();
}

int main() {
    test_all();

    return 0;
}
