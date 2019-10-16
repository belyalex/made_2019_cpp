#ifndef CALC_H
#define CALC_H

enum TOKEN {
	UNKNOWN,
	EOL,
	NUMBER,
	ADD,
	SUB,
	MUL,
	DIV
};

enum STATE {
	STATE_OK,
	STATE_NULLPTR,
	STATE_UNEXPECTED_STRINGEND,
	STATE_UNEXPECTED_CHAR,
	STATE_OVERFLOW,
	STATE_NUMBER_EXPECTED,
	STATE_DIVISION_BY_ZERO,
};

class calc 
{
public:	
	int eval(const char* s);
	STATE get_state() const { return state; };
	const char* get_message() const { return messages[get_state()]; };

private:
	static const char* messages[];
	STATE state=STATE_OK;
	const char* pos=nullptr;
	long long int number = 0;
	TOKEN curr_token = UNKNOWN;

	int eval_sum_sub();
	int eval_mul_div();
	int eval_neg_number();
	int eval_number();

	TOKEN get_next_token();
	char get_next_char();
	char get_curr_char() const { return *pos; }

	bool test_int(long long int n) const;
};

#endif
