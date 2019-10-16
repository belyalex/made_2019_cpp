#include <iostream>
#include <climits>
#include <string>
#include "calc.h"

#define TESTS_BEGIN \
	int N_TESTS = 0; \
	int N_FAILED_TESTS = 0; 

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

void test() {
	TESTS_BEGIN
	calc c;
	//correct expressions
	TEST((c.eval("2*2") == 4) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("2+2") == 4) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("2-2") == 0) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("-2147483648") == INT_MIN) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("2147483647") == INT_MAX) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval(" 1  +  2+3  + 4   +   5+6+7+8 +  9+10") == 55) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("-1  -  2-3  - 4   -   5-6-7-8 -  9-10") ==-55) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("1 * 2*3*4*5   *   6*7*8 *9*   10  ") == 3628800) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("255 /- 2") == -127) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval("255/15") == 17) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	//12345679*8==98765432
	TEST((c.eval(" 12345679   * 8 ") == 98765432) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval(" 256  * 256 * 256 ") == 16777216) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	TEST((c.eval(" - 256  * - 256 * -256 ") == -16777216) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	//division by zero 
	TEST((c.eval(" 1+2*3/4-5*6+7*9/0 ") == 0) && (c.get_state() == STATE_DIVISION_BY_ZERO))
	TEST(std::string(c.get_message())=="Division by zero")
	//overflow
	TEST((c.eval(" 256 * 256 * 256 * 256 ") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST(std::string(c.get_message())=="int type overflow")
	TEST((c.eval("-2147483647 - 1 -1") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST(std::string(c.get_message())=="int type overflow")
	TEST((c.eval("-2147483648/ - 1") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST(std::string(c.get_message())=="int type overflow")
	TEST((c.eval("-2147483648 * - 1") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST(std::string(c.get_message())=="int type overflow")
	TEST((c.eval("2147483648") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST(std::string(c.get_message())=="int type overflow")
	//2 unary minus not allowed
	TEST((c.eval(" - 256  * - 256 * -  -  256 ") == 0) && (c.get_state() == STATE_NUMBER_EXPECTED))
	TEST(std::string(c.get_message())=="Number expected")
	//unary plus not allowed
	TEST((c.eval(" - 256  * - 256 * + 256 ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval(" -256 - + 256 ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	//INT_MIN
	TEST((c.eval("-2147483647 - 1") == INT_MIN) && (c.get_state() == STATE_OK))
	TEST(std::string(c.get_message())=="OK")
	//incorrect expressions
	TEST((c.eval(nullptr) == 0) && (c.get_state() == STATE_NULLPTR))
	TEST(std::string(c.get_message())=="Expression is nullptr")
	TEST((c.eval("") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("         ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  -   ") == 0) && (c.get_state() == STATE_NUMBER_EXPECTED))
	TEST(std::string(c.get_message())=="Number expected")
	TEST((c.eval("  +   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  *   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  /   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  1 1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  1.1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  1**1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  1//1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  a   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TEST((c.eval("  1+   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST(std::string(c.get_message())=="Unexpected character")
	TESTS_END
}

int main() {
	test();

	return 0;
}
