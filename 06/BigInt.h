#ifndef BIGINT_H
#define BIGINT_H

#include <ostream>

class BigInt {
public:
	BigInt(const int value=0);
	BigInt& operator=(const int value);

	~BigInt() { delete[] buffer; }
	BigInt(const BigInt& value);
	BigInt(BigInt&& value);	
	BigInt& operator=(const BigInt& value);
	BigInt& operator=(BigInt&& value);

	bool operator==(const BigInt& v);
	bool operator!=(const BigInt& v) { return ! (*this == v); }
	bool operator<(const BigInt& v);
	bool operator<=(const BigInt& v) { return (*this < v) || (*this == v); }
	bool operator>=(const BigInt& v) { return !(*this < v); }
	bool operator>(const BigInt& v) { return !(*this < v) && !(*this == v); }

	BigInt operator+(const BigInt& v) const;
	BigInt operator-(const BigInt& v) const;

	BigInt operator-() const;

	friend std::ostream& operator<<(std::ostream& os, const BigInt& value);
private:
	void grow_buffer(const int delta = CAPACITY_DELTA, const bool copy_buf = false);
	void int2BigInt(int value);
	static const int CAPACITY_DELTA = 8;
	size_t capacity = 0;
	size_t size = 0;
	bool is_negative = false;
	char* buffer = nullptr;
};


#endif

