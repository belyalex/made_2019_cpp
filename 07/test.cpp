#include <iostream>
#include <string.h>
#include "format.h"


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

void Test() {
        TESTS_BEGIN("format() tests")
            TEST(format("{1}+{1} = {0}", 2, "one")=="one+one = 2")
        try {
            format("{}+{1} = {0}", 2, "one") == "{}+one = 2";
        }
        catch (const std::runtime_error& e) {
            TEST(strcmp(e.what(),"Expected number in {} placeholder.")==0)
        }
        try {
            format("{one}+{1} = {0}", 2, "one") == "{one}+one = 2";
        }
        catch (const std::runtime_error& e) {
            TEST(strcmp(e.what(),"Expected number in {} placeholder.")==0)
        }
        try {
            format("{1+{1} = {0}", 2, "one") == "{1+one = 2";
        }
        catch (const std::runtime_error& e) {
            TEST(strcmp(e.what(),"Expected '}'.")==0)
        }
        try {
            format("{2}+{1} = {0}", 2, "one") == "{2}+one = 2";
        }
        catch (const std::runtime_error& e) {
            TEST(strcmp(e.what(),"Placeholder number is too much.")==0)
        }
        try {
            format("{1}+{1} }= {0}", 2, "one") == "one+one }= 2";
        }
        catch (const std::runtime_error& e) {
            TEST(strcmp(e.what(),"Unexpected '}'.")==0)
        }
        TESTS_END
}

int main() {
    Test();

    return 0;
}
