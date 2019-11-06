#include <iostream>
#include "matrix.h"
 
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



void Test0() {
	const size_t rows = 5;
	const size_t cols = 3;
	Matrix m(rows, cols);
	Matrix m1(cols, rows);
	TESTS_BEGIN("Matrix getRows & getColumns tests")
		TEST(m.getRows() == rows)
		TEST(m.getColumns() == cols)
		TEST(m1.getRows() == cols)
		TEST(m1.getColumns() == rows)
	TESTS_END
}

void Test1() {
	const size_t rows = 5;
	const size_t cols = 3;
	Matrix m(rows, cols);
	TESTS_BEGIN("Matrix operator[][] tests")
		for (size_t i = 0; i < m.getRows(); i++) {
			for (size_t j = 0; j < m.getColumns(); j++) {
				TEST(m[i][j] == 0);
				m[i][j] = i*m.getColumns() + j;
				TEST(m[i][j] == i*m.getColumns() + j);
			}
		}

		for (int i = 0; i < m.getRows(); i++) {
			for (int j = 0; j < m.getColumns(); j++) {
				TEST(m[i][j] == i*m.getColumns() + j);
			}
		}
		try {
		    TEST((m[5][0]=0,false));
		}
		catch(std::out_of_range& e) {
		    TEST(("m[5][0]=0 - out_of_range",true));
		}
		try {
		    TEST((m[0][3]=0,false));
		}
		catch(std::out_of_range& e) {
		    TEST(("m[0][3]=0 - out_of_range",true));
		}
	TESTS_END
}

void Test2() {
	const size_t rows = 5;
	const size_t cols = 3;
	Matrix m(rows, cols);
	Matrix m1(rows, cols);
	Matrix m2(cols, rows);

	TESTS_BEGIN("Matrix operator== & operator!= tests")
		TEST(m == m)
		TEST(m == m1)
		TEST(m != m2)
		TEST(m1 != m2)
		TEST((m[4][2] = 42,  m != m1))		
		TEST((m1[4][2] = 42, m == m1))
	TESTS_END
}

void Test3() {
	const size_t rows = 5;
	const size_t cols = 3;
	Matrix m(rows, cols);
	Matrix m42(rows, cols);

	int p=0;
	for (size_t i = 0; i < m.getRows(); i++) {
		for (size_t j = 0; j < m.getColumns(); j++) {			
			m42[i][j] = p * 42;
			m[i][j] = p++;
		}
	}
	TESTS_BEGIN("Matrix operator*= tests")
		TEST(m != m42)
		TEST((m *= 42, m == m42))
		TEST((m *= 2,  m != m42))
		TEST((m42 *= 2, m == m42))
		TEST((m *= 3,   m != m42))
		TEST((m42 *= 3, m == m42))
		TEST((m *= 10,  m != m42))
		TEST((m42 *= 10, m == m42))		
	TESTS_END
}

void Test4() {
	const size_t rows = 5;
	const size_t cols = 3;
	const Matrix m(rows, cols);
	//Если раскомментировать следующие строки, не пройдёт компиляция - попытка изменить константную матрицу.
	//m[4][2]=1;
	//m*=2;
}

void TestAll() {
	Test0();

	Test1();

	Test2();

	Test3();

	Test4();
}

int main() {
	TestAll();

	return 0;
}