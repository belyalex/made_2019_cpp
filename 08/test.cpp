#include <iostream>
#include <sstream>
#include "Vector.h"

#define TESTS_BEGIN(c) \
    int N_TESTS = 0; \
    int N_FAILED_TESTS = 0; \
    c; \
    std::cout << "Tests " << #c << " started." << std::endl;

#define TEST(c)  do { \
    N_TESTS++; \
    if (!(c)) { \
    N_FAILED_TESTS++; \
        std::cout << "Test " << #c << " failed in " << __FILE__ \
                  << "(line " << __LINE__ << ") : " << std::endl; \
    } \
    else { \
        std::cout << "Test " << #c << " done successfully" << std::endl; \
    } \
} while(0);

#define TESTS_END \
    std::cout << "Failed " << N_FAILED_TESTS << " from " << N_TESTS << " tests." << std::endl << std::endl;

void TestConstructors() {
    TESTS_BEGIN("Vector constructors tests")
    Vector<int> v;
    TEST(v.size() == 0 && v.capacity() == INITIAL_CAPACITY)
    Vector<int> v1(10);
    TEST(v1.size() == 10 && v1.capacity() == 10 && v1[0] == 0)
    Vector<int> v_copy(v1);
    TEST(v_copy.size() == v1.size() && v_copy.capacity() == v1.capacity() && v_copy[5] == v1[5])
    Vector<int> v2(20, 42);
    TEST(v2.size() == 20 && v2.capacity() == 20 && v2[13] == 42)
    Vector<int> v_move(std::move(v2));
    TEST(v_move.size() == 20 && v_move.capacity() == 20 && v_move[13] == 42 &&
         v2.size() == 0 && v2.capacity() == 0)
    Vector<std::string> vs(5, "aaa");
    TEST(vs.size() == 5 && vs.capacity() == 5 && vs[4] == "aaa")
    Vector<std::string> vs1{"раз", "два", "три"};
    TEST(vs1.size() == 3 && vs1.capacity() == 3 && vs1[0] == "раз" && vs1[1] == "два" && vs1[2] == "три")
    TESTS_END
}

void TestAssigments() {
    TESTS_BEGIN("Vector assigments tests")
    Vector<double> v;
    TEST(v.size() == 0 && v.capacity() == INITIAL_CAPACITY)
    Vector<double> v1(10, 0.42);
    v = v1;
    TEST(v.size() == v1.size() && v.capacity() == v1.capacity() && v[5] == v1[5])
    Vector<double> v2(20, 3.14);
    v = std::move(v2);
    TEST(v.size() == 20 && v.capacity() == 20 && v[13] == 3.14 &&
         v2.size() == 0 && v2.capacity() == 0)
    TESTS_END
}

void TestResize() {
    TESTS_BEGIN("Vector resize tests")
    Vector<int> v(10, 42);
    TEST(v.size() == 10 && v.capacity() == 10 && v[9] == 42)
    v.resize(20, 24);
    TEST(v.size() == 20 && v.capacity() == 20 && v[10] == 24)
    v.resize(5);
    TEST(v.size() == 5 && v.capacity() == 20 && v[4] == 42)
    v.resize(10);
    TEST(v.size() == 10 && v.capacity() == 20 && v[9] == 0)
    TESTS_END
}

void TestReserve() {
    TESTS_BEGIN("Vector reserve tests")
    Vector<int> v;
    TEST(v.size() == 0 && v.capacity() == INITIAL_CAPACITY)
    v.reserve(20);
    TEST(v.size() == 0 && v.capacity() == 20)
    v.reserve(10);
    TEST(v.size() == 0 && v.capacity() == 20)
    TESTS_END
}

void TestPushPop() {
    TESTS_BEGIN("Vector push-pop tests")
    Vector<int> v;
    TEST(v.size() == 0 && v.capacity() == INITIAL_CAPACITY)
    v.push_back(1);
    TEST(v.size() == 1 && v.capacity() == INITIAL_CAPACITY && v[0] == 1)
    for (size_t i = 2; i < 20; i++) {
        v.push_back(i);
    }
    TEST(v.size() == 19 && v.capacity() == INITIAL_CAPACITY * 2 && v[18] == 19)
    for (size_t i = 0; i < 10; i++) {
        v.pop_back();
    }
    TEST(v.size() == 9 && v.capacity() == INITIAL_CAPACITY * 2 && v[8] == 9)
    v.clear();
    TEST(v.size() == 9 && v.capacity() == INITIAL_CAPACITY * 2)
    TESTS_END
}

void TestIndexing() {
    TESTS_BEGIN("Vector indexing tests")
    Vector<int> v(10, 10);
    TEST(v[5] == 10)
    v[5] = 42;
    TEST(v[5] == 42)
    const Vector<int> x(10, 10);
    TEST(x[5] == 10)
    //x[5]=42; //эта строка не скомпилится, если её раскомментировать
    TESTS_END
}

void TestIterators() {
    TESTS_BEGIN("Vector iterators tests")
    std::stringstream ss;
    Vector<int> v(5, 42);
    for (const auto &value : v) {
        ss << value << ' ';
    }
    TEST(ss.str() == "42 42 42 42 42 ")
    ss.str("");
    v.resize(10, -42);
    for (auto it = v.rbegin(); it != v.rend(); it++) {
        ss << *it << ' ';
    }
    TEST(ss.str() == "-42 -42 -42 -42 -42 42 42 42 42 42 ")
    ss.str("");
    for (auto it = v.begin(); it < v.end(); it += 2) {
        ss << *it << ' ';
    }
    TEST(ss.str() == "42 42 42 -42 -42 ")
    TESTS_END
}

void TestAll() {
    TestConstructors();

    TestAssigments();

    TestResize();

    TestReserve();

    TestPushPop();

    TestIndexing();

    TestIterators();
}

int main() {
    TestAll();

    return 0;
}
