#include <iostream>
#include <sstream>

#include "serializer.h"

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

struct Data
{
	uint64_t a;
	bool b;
	uint64_t c;

	template <class Serializer>
	Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c);
	}

	bool operator==(Data d) {
		if (this == &d) return true;
		return (a == d.a && b == d.b && c == d.c);
	}
};

struct Data2
{
	bool a;
	uint64_t b;
	bool c;

	template <class Serializer>
	Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c);
	}
};

void Test0() {
	std::stringstream ss;
	Data d{ 123456, true, 987654321 };	
	Data d1{ 24680, false, 1234567890 };

	Serializer ser(ss);

	TESTS_BEGIN("Serializer")
		TEST(ser.save(d) == Error::NoError)
		TEST(ss.str() == "123456 true 987654321 ")
		TEST(ser.save(d1) == Error::NoError)
		TEST(ss.str() == "123456 true 987654321 24680 false 1234567890 ")
	TESTS_END
}

void Test1() {
	
	Data d;
	Data d1{ 123456, true, 987654321 };
	Data d2{ 24680, false, 1234567890 };

	std::stringstream ss;
	Deserializer deser(ss);

	TESTS_BEGIN("Deserializer")
		TEST((ss.str("123456 true 987654321 "), deser.load(d) == Error::NoError))
		TEST(d==d1)
		TEST((ss.str("24680 false 1234567890 "),deser.load(d) == Error::NoError))
		TEST(d == d2)

		TEST((ss.str(""),deser.load(d) == Error::CorruptedArchive))
		TEST((ss.str("1 1 1"),deser.load(d) == Error::CorruptedArchive))		
		TEST((ss.str("true"),deser.load(d) == Error::CorruptedArchive))
	TESTS_END
}

void Test2() {
	std::stringstream ss;
	Data d;
	Data d1{ 123456,true,987654321 };
	Data d2{ 24680, false, 1234567890 };
	Data2 x;

	Serializer ser(ss);
	Deserializer deser(ss);

	TESTS_BEGIN("Serializer & Deserializer")
		TEST(ser.save(d1) == Error::NoError)
		TEST(deser.load(d) == Error::NoError)
		TEST(d == d1)

		TEST(ser.save(d2) == Error::NoError)
		TEST(deser.load(d) == Error::NoError)
		TEST(d == d2)

		ss.str("");
		TEST(ser.save(d1) == Error::NoError)
		TEST(deser.load(x) == Error::CorruptedArchive)
		ss.str("");
		TEST(ser.save(d2) == Error::NoError)
		TEST(deser.load(x) == Error::CorruptedArchive)
	TESTS_END
}

void TestAll() {
	Test0();

	Test1();

	Test2();
}

int main() {
	TestAll();

	return 0;
}
