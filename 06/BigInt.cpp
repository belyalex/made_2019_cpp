#include <algorithm>
#include <cstring>
#include <cmath>
//#include <iostream>
#include "BigInt.h"

BigInt::BigInt(const int value)
{
	int2BigInt(value);
}

BigInt& BigInt::operator=(const int value)
{
	int2BigInt(value);

	return *this;
}
//копирование
BigInt::BigInt(const BigInt & value) :
	buffer(new char[value.capacity]),
	capacity(value.capacity),
	size(value.size),
	is_negative(value.is_negative)
{	
	std::memcpy(&buffer[capacity - size], &value.buffer[capacity - size], size);
}
//перемещение
BigInt::BigInt(BigInt && value) :
	buffer(value.buffer),
	capacity(value.capacity),
	size(value.size),
	is_negative(value.is_negative)
{
	value.buffer=nullptr;
	value.capacity = 0;
	value.size = 0;
	value.is_negative = false;
}

//присваивание
BigInt& BigInt::operator=(const BigInt& value)
{
	if (this == &value) return *this;

	char* new_buf = new char[value.capacity];

	is_negative = value.is_negative;
	capacity = value.capacity;
	size = value.size;	

	delete buffer;
	buffer = new_buf;
	std::memcpy(&buffer[capacity - size], &value.buffer[capacity - size], size);

	return *this;
}

//перемещение
BigInt& BigInt::operator=(BigInt&& value)
{
	if (this == &value) return *this;

	delete buffer;
	buffer = value.buffer;
	capacity = value.capacity;
	size = value.size;
	is_negative = value.is_negative;

	value.buffer = nullptr;
	value.capacity = 0;
	value.size = 0;
	value.is_negative = false;

	return *this;
}

bool BigInt::operator==(const BigInt& v)
{
	if (this == &v) return true;

	if (size != v.size) return false;
	if (is_negative != v.is_negative) return false;

	return std::memcmp(&buffer[capacity-size], &v.buffer[v.capacity - v.size], size)==0;
}

bool BigInt::operator<(const BigInt& v)
{
	if (this == &v) return false;

	if (is_negative != v.is_negative) return is_negative;
	if (size != v.size) return is_negative ? size > v.size : size<v.size;	
	return is_negative ?  std::memcmp(&buffer[capacity - size], &v.buffer[v.capacity - v.size], size)>0:
						  std::memcmp(&buffer[capacity - size], &v.buffer[v.capacity - v.size], size)<0;
}

BigInt BigInt::operator+(const BigInt & v) const
{
	BigInt res;
	size_t r = (std::max(size, v.size) / CAPACITY_DELTA + 1) * CAPACITY_DELTA;
	if (r > res.capacity) {
		res.grow_buffer(r - res.capacity);
	}
	res.size = 0;	
	if (is_negative == v.is_negative) {
		int p = 0; //перенос
		for (size_t s = 0; s < std::min(size,v.size); s++) {
			int r = buffer[capacity - s - 1] - '0' + v.buffer[v.capacity - s - 1] - '0' + p;
			if (r > 9) {
				p = 1;
				r -= 10;
			}
			else {
				p = 0;
			}
			res.buffer[res.capacity - s - 1] = '0' + r;
			res.size++;
		}
		for (size_t s = std::min(size, v.size); s < std::max(size, v.size);s++) {
			int r = size > v.size ? buffer[capacity - s - 1] - '0' + p : v.buffer[v.capacity - s - 1] - '0' + p;
			if (r > 9) {
				p = 1;
				r -= 10;
			}
			else {
				p = 0;
			}
			res.buffer[res.capacity - s - 1] = '0' + r;
			res.size++;
		}		
		if (p > 0) {
			res.buffer[res.capacity - ++res.size] = '1';
		}
		res.is_negative = is_negative;
	}
	else {
		int p = 0; //перенос
		if ((size > v.size) || (std::memcmp(&buffer[capacity - size], &v.buffer[v.capacity - v.size], size) > 0)) {
			for (size_t s = 0; s < std::min(size, v.size); s++) {
				int r = buffer[capacity - s - 1] - '0' - (v.buffer[v.capacity - s - 1] - '0') + p;
				if (r < 0) {
					p = -1;
					r += 10;
				}
				else {
					p = 0;
				}
				res.buffer[res.capacity - s - 1] = '0' + r;
				res.size++;
			}
			for (size_t s = std::min(size, v.size); s < std::max(size, v.size); s++) {
				int r = size > v.size ? buffer[capacity - s - 1] - '0' + p : -(v.buffer[v.capacity - s - 1] - '0') + p;
				if (r < 0) {
					p = -1;
					r += 10;
				}
				else {
					p = 0;
				}
				res.buffer[res.capacity - s - 1] = '0' + r;
				res.size++;
			}
			res.is_negative = is_negative;
		}
		else {
			for (size_t s = 0; s < std::min(size, v.size); s++) {
				int r = v.buffer[capacity - s - 1] - '0' - (buffer[v.capacity - s - 1] - '0') + p;
				if (r < 0) {
					p = -1;
					r += 10;
				}
				else {
					p = 0;
				}
				res.buffer[res.capacity - s - 1] = '0' + r;
				res.size++;
			}
			for (size_t s = std::min(size, v.size); s < std::max(size, v.size); s++) {
				int r = size > v.size ? buffer[capacity - s - 1] - '0' + p : v.buffer[v.capacity - s - 1] - '0' + p;
				if (r < 0) {
					p = -1;
					r += 10;
				}
				else {
					p = 0;
				}
				res.buffer[res.capacity - s - 1] = '0' + r;
				res.size++;
			}
			res.is_negative = v.is_negative;
		}
	}
	while ((res.size>1) && (res.buffer[res.capacity - res.size] == '0')) {
		res.size--;
	}
	if ((res.size == 1) && (res.buffer[res.capacity - 1] == '0')) {
		res.is_negative = false;
	}
	return res;
}

BigInt BigInt::operator-(const BigInt & v) const
{
	return *this+-v;
}

BigInt BigInt::operator-() const
{
	BigInt res = *this;
	res.is_negative = !is_negative;

	return res;
}

void BigInt::grow_buffer(const int delta, const bool copy_buf)
{	
	char* new_buf = new char[capacity + delta];
	if (copy_buf) {
		std::memcpy(&new_buf[capacity + delta - size], &buffer[capacity - size], size);
	}	
	delete[] buffer;
	buffer = new_buf;
	capacity += delta;
}

void BigInt::int2BigInt(int value)
{
	size = 0;
	is_negative = false;
	if (value < 0) {
		is_negative = true;
		value = -value;
	}
	
	if (value == 0) {
		if (capacity == 0) {
			grow_buffer(CAPACITY_DELTA);
		}
		buffer[capacity - 1] = '0';
		size = 1;
		return;
	}

	size_t r = trunc(log(fabs((double)value)) / log(10.0) + 1);
	//std::cout<<r<<std::endl;
	//std::cout<<log(fabs((double)value))<<std::endl;

	if (r > capacity) {
		grow_buffer(((r - capacity) / CAPACITY_DELTA + 1)*CAPACITY_DELTA);
	}
	
	while (value != 0) {
		buffer[capacity - (++size)] = '0' + value % 10;
		value /= 10;
		//if (capacity==size) {
		//    std::cout<<"grow\n";
		//   grow_buffer(CAPACITY_DELTA, true);
		//}
	}
}

std::ostream& operator<<(std::ostream& os, const BigInt& value)
{
	if (value.is_negative) {
		os << '-';
	}
	for (size_t s = 0; s < value.size; s++) {
		os << value.buffer[value.capacity - value.size + s];
	}

	return os;
}

