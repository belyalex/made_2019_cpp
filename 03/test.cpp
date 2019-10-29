#include <iostream>
#include <sstream>
#include "tokenizer.h"

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

std::stringstream ss;
void test0() {    
    auto f1=[]() mutable {ss<<"Begin tokenize"<<std::endl;};
    auto f2=[]() mutable {ss<<"End tokenize"<<std::endl;};
    auto f3=[](int token) mutable {ss<<"int:"<<token<<std::endl;};
    auto f4=[](const std::string& token) mutable {ss<<"string:"<<token<<std::endl;};
    Tokenizer t(
	f1,
	f2,
	f3,
	f4
    );
    
    TESTS_BEGIN("all callbacks");
    ss.str("");
    t.tokenize("");
    TEST(ss.str()=="Begin tokenize\nEnd tokenize\n");

    ss.str("");
    t.tokenize("asdf 1234 fdsa\t367233\n1267216721dshsddsj 1216372163721613712637");
    TEST(ss.str()=="Begin tokenize\nstring:asdf\nint:1234\nstring:fdsa\nint:367233\nstring:1267216721dshsddsj\nstring:1216372163721613712637\nEnd tokenize\n");
    
    TESTS_END
}

void test1() {
    auto f3=[](int token) mutable {ss<<"int:"<<token<<std::endl;};
    auto f4=[](const std::string& token) mutable {ss<<"string:"<<token<<std::endl;};
    Tokenizer t(
	nullptr,
	nullptr,
	f3,
	f4
    );
    
    TESTS_BEGIN("only token callbacks");
    ss.str("");
    t.tokenize("");
    TEST(ss.str()=="");

    ss.str("");
    t.tokenize("asdf 1234 fdsa\t367233\n1267216721dshsddsj 1216372163721613712637");
    TEST(ss.str()=="string:asdf\nint:1234\nstring:fdsa\nint:367233\nstring:1267216721dshsddsj\nstring:1216372163721613712637\n");
    
    TESTS_END
}

void test2() {
    auto f4=[](const std::string& token) mutable {ss<<"string:"<<token<<std::endl;};
    Tokenizer t(
	nullptr,
	nullptr,
	nullptr,
	f4
    );
    
    TESTS_BEGIN("only string token callback");
    ss.str("");
    t.tokenize("");
    TEST(ss.str()=="");

    ss.str("");
    t.tokenize("asdf 1234 fdsa\t367233\n1267216721dshsddsj 1216372163721613712637");
    TEST(ss.str()=="string:asdf\nstring:fdsa\nstring:1267216721dshsddsj\nstring:1216372163721613712637\n");
    
    TESTS_END
}

void test3() {
    auto f3=[](int token) mutable {ss<<"int:"<<token<<std::endl;};
    Tokenizer t(
	nullptr,
	nullptr,
	f3,
	nullptr
    );
    
    TESTS_BEGIN("only int token callback");
    ss.str("");
    t.tokenize("");
    TEST(ss.str()=="");

    ss.str("");
    t.tokenize("asdf 1234 fdsa\t367233\n1267216721dshsddsj 1216372163721613712637");
    TEST(ss.str()=="int:1234\nint:367233\n");
    
    TESTS_END
}

void test_all() {
    test0();

    test1();

    test2();

    test3();
}

int main() {
    test_all();

    return 0;
}
