#include <iostream>
#include <climits>
#include <sstream>

#include "BigInt.h"

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
	std::cout << "Failed " << N_FAILED_TESTS << " from " << N_TESTS << " tests." << std::endl << std::endl;

template<unsigned int N>
struct fibb {
	static const unsigned int value = fibb<N - 1>::value + fibb<N - 2>::value;
};

template<>
struct fibb<0> {
	static const unsigned int value = 1;
};

template<>
struct fibb<1> {
	static const unsigned int value = 1;
};

void Test0() {
	BigInt x(1);
	BigInt y;
	BigInt z(1);
	TESTS_BEGIN("Tests ==")
		TEST(("x(1), y ", !(x == y)))
		TEST(("x(1), z(1) ", (x == z)))
		TEST((x = 98765432, y = 12345679 * 8, x == y))
		TEST((x = 98765432, y = -12345679 * 8, !(x == y)))
		TEST((x = (1 << 31), y = (1 << 31), x == y))
		TEST((x = INT_MAX, y = INT_MAX, x == y))
		TEST((x = INT_MIN, y = INT_MIN, x == y))
		TEST((x = INT_MIN, y = INT_MAX, !(x == y)))
		TEST((x = INT_MIN, y = -INT_MAX, !(x == y)))
		TEST((x = INT_MIN, y = -INT_MAX - 1, (x == y)))
		TEST((x = -(INT_MIN + 1), y = INT_MAX, (x == y)))
		TEST((x = 13, (x == fibb<6>::value)))
		TEST((x = 165580141, (x == fibb<40>::value)))
	TESTS_END
}

void Test1() {
	BigInt x(1);
	BigInt y;
	BigInt z(1);
	TESTS_BEGIN("Tests !=")
		TEST(("BigInt x(1), y ", (x != y)))
		TEST(("BigInt x(1), z(1) ", !(x != z)))
		TEST((x = 98765432, y = 12345679 * 8, !(x != y)))
		TEST((x = 98765432, y = -12345679 * 8, (x != y)))
		TEST((x = (1 << 31), y = (1 << 31), !(x != y)))
		TEST((x = INT_MAX, y = INT_MAX, !(x != y)))
		TEST((x = INT_MIN, y = INT_MIN, !(x != y)))
		TEST((x = INT_MIN, y = INT_MAX, (x != y)))
		TEST((x = INT_MIN, y = -INT_MAX, (x != y)))
		TEST((x = INT_MIN, y = -INT_MAX - 1, !(x != y)))
		TEST((x = -(INT_MIN + 1), y = INT_MAX, !(x != y)))
		TEST((x = 13, !(x != fibb<6>::value)))
		TEST((x = 165580141, !(x != fibb<40>::value)))
	TESTS_END
}

void Test2() {
	BigInt x, y;
	TESTS_BEGIN("Tests <")
		TEST(("BigInt x, y", !(x < y)))
		TEST((x = 0, y = 1, (x < y)))
		TEST((x = 0, y = 1, !(y < x)))
		TEST((x = -1, y = 0, (x < y)))
		TEST((x = -1, y = 0, !(y < x)))
		TEST((x = -1, y = 1, (x < y)))
		TEST((x = -1, y = 1, !(y < x)))
	TESTS_END
}

void Test3() {
	BigInt x, y;
	TESTS_BEGIN("Tests <=")
		TEST(("BigInt x, y", (x <= y)))
		TEST((x = 0, y = 1, (x <= y)))
		TEST((x = 0, y = 1, !(y <= x)))
		TEST((x = -1, y = 0, (x <= y)))
		TEST((x = -1, y = 0, !(y <= x)))
		TEST((x = -1, y = 1, (x <= y)))
		TEST((x = -1, y = 1, !(y <= x)))
	TESTS_END
}

void Test4() {
	BigInt x, y;
	TESTS_BEGIN("Tests >")
		TEST(("BigInt x, y", !(x > y)))
		TEST((x = 0, y = 1, !(x > y)))
		TEST((x = 0, y = 1, (y > x)))
		TEST((x = -1, y = 0, !(x > y)))
		TEST((x = -1, y = 0, (y > x)))
		TEST((x = -1, y = 1, !(x > y)))
		TEST((x = -1, y = 1, (y > x)))
	TESTS_END
}

void Test5() {
	BigInt x, y;
	TESTS_BEGIN("Tests >=")
		TEST(("BigInt x, y", (x >= y)))
		TEST((x = 0, y = 1, !(x >= y)))
		TEST((x = 0, y = 1, (y >= x)))
		TEST((x = -1, y = 0, !(x >= y)))
		TEST((x = -1, y = 0, (y >= x)))
		TEST((x = -1, y = 1, !(x >= y)))
		TEST((x = -1, y = 1, (y >= x)))
	TESTS_END
}

void Test6() {
	BigInt x(1), y(-1);
	TESTS_BEGIN("Tests unary minus")
		TEST(("BigInt x(1), y(-1)", (x == -y)))
		TEST((x = 10, y = -10, (-x == y)))
		TEST((x = INT_MIN + 1, y = INT_MAX, (-x == y)))		
	TESTS_END
}

void Test7() {
	BigInt x(1), y(-1), z;
	TESTS_BEGIN("Tests binary plus")
		TEST(("BigInt x(1), y(-1)", (x + y == 0)))
		//std::cout << x + y << std::endl;
		TEST((x = -10, y = 9, (x + y == -1)))
		TEST((x = 10, y = -9, (x + y == 1)))
		TEST((x = 1, y = 9, (x + y == 10)))
		TEST((x = 9, y = 91, (x + y == 100)))
		TEST((x = -99, y = -1, (x + y == -100)))
		TEST((x = fibb<40>::value, y = fibb<41>::value, (x + y == fibb<42>::value)))
		TEST((x = -fibb<40>::value, y = fibb<41>::value, (x + y == fibb<39>::value)))
		//std::cout <<x<<" "<<y<<" "<< x + y << std::endl;
	TESTS_END
}

void Test8() {
	BigInt x(1), y(-1), z;
	TESTS_BEGIN("Tests binary minus")
		TEST(("BigInt x(1), y(-1)", (x - y == 2)))
		TEST((x = -10, y = 9, (x - y == -19)))
		TEST((x = 10, y = -9, (x - y == 19)))
		TEST((x = 1, y = 9, (x - y == -8)))
		TEST((x = 9, y = 91, (x - y == -82)))
		//std::cout << x - y << std::endl;
		TEST((x = -99, y = -1, (x - y == -98)))
		TEST((x = fibb<40>::value, y = fibb<41>::value, (x - y == -fibb<39>::value)))
		TEST((x = -fibb<40>::value, y = fibb<41>::value, (x - y == -fibb<42>::value)))
		//std::cout << x << " " << y << " " << x + y << std::endl;
	TESTS_END
}

void Test9() {
	std::stringstream ss;
	BigInt x(1);
	TESTS_BEGIN("Test operator<<")
	    ss<<x;
	    TEST(ss.str()=="1")
	    ss.str("");
	    x=-100;
	    ss<<x;
	    TEST(ss.str()=="-100")
	    
	    ss.str("");
	    x=1;
	    for (int i=0;i<64;i++) {
		x=x+x;
	    }
	    x=x-1;
	    ss<<x;
	    //std::cout<<ss.str()<<std::endl;
	    TEST(ss.str()=="18446744073709551615");
	TESTS_END
}

void TestAll() {
	Test0();

	Test1();

	Test2();

	Test3();

	Test4();

	Test5();

	Test6();

	Test7();

	Test8();

	Test9();
}

int main() {
	TestAll();

	return 0;
}
