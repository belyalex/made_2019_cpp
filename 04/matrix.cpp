#include "matrix.h"
#include <cstring>

Matrix::Matrix(size_t rows, size_t cols):
	rows(rows),
	cols(cols)
{
	buf = new int[rows * cols]{ 0 };	
}

bool Matrix::operator==(const Matrix& m) const
{
	if (this == &m) return true;

	if (rows != m.rows) return false;
	if (cols != m.cols) return false;

	return std::memcmp(buf, m.buf, rows * cols * sizeof(int)) == 0;	
}

Matrix& Matrix::operator*=(const int x)
{
	for (int i = 0; i < rows * cols; i++) {
		buf[i] *= x;
	}
	return *this;
}

Matrix::ProxyRow Matrix::operator[](const size_t row) const
{
	if (row >= rows) {
	    throw std::out_of_range("");
	}

	ProxyRow pr(&buf[row * cols],cols);
	return pr;
}

int& Matrix::ProxyRow::operator[](const size_t col) const
{
	if (col >= cols) {
	    throw std::out_of_range("");
	}

	return buf[col];
}
