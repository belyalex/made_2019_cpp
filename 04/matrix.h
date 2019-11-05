#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <stdexcept>

class Matrix {
	class ProxyRow;
public:
	Matrix(size_t rows, size_t cols);
	~Matrix() { delete[] buf; };

	bool operator==(const Matrix& m) const;
	bool operator!=(const Matrix& m) const { return !(*this == m); };
	Matrix& operator*=(const int x);
	ProxyRow operator[](const size_t row) const;
	int getRows() const { return rows; };
	int getColumns() const { return cols; };
private:
	int* buf=nullptr;
	size_t rows;
	size_t cols;

	class ProxyRow {	
	public:
		ProxyRow(int* buf, size_t cols) : cols(cols),buf(buf) {};
		int& operator[](const size_t col) const;
	private:		
		int* buf;
		size_t cols;
	};
};

#endif