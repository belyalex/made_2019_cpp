#include <iostream>
#include <climits>
#include "calc.h"

const char* calc::messages[] = {
	"OK",
	"Expression is nullptr",
	"Unexpected end of expression",
	"Unexpected character",
	"int type overflow",
	"Number expected",
	"Division by zero",
};

int calc::eval(const char* s) {	
	pos = s;
	state = STATE_OK;
	number = 0;
	curr_token = UNKNOWN;
	if (s == nullptr) {
		state = STATE_NULLPTR;
		return 0;
	}
	return eval_sum_sub();		
}

int calc::eval_sum_sub() {
	long long int res = eval_mul_div();

	while (STATE_OK == state) {
		switch (get_next_token()) {
		case ADD:
			res += eval_mul_div();			
			if (!test_int(res)) {
				state = STATE_OVERFLOW;
				return 0;
			}
			break;
		case SUB:
			res -= eval_mul_div();			
			if (!test_int(res)) {
				state = STATE_OVERFLOW;
				return 0;
			}
			break;
		case EOL:
			return static_cast<int>(res);
		default:
			state = STATE_UNEXPECTED_CHAR;
			return 0;
		}
	}

	return 0;
}

int calc::eval_mul_div() {
	long long int res = eval_neg_number();
	int divisor;
	while (STATE_OK == state) {
		switch (TOKEN token=get_next_token()) {
		case MUL:			
			res *= eval_neg_number();			
			if (!test_int(res)) {
				state = STATE_OVERFLOW;
				return 0;
			}
			break;
		case DIV:			
			divisor = eval_neg_number();
			if (STATE_OK != state) {
				return 0;
			}
			if (0 == divisor) {
				state = STATE_DIVISION_BY_ZERO;
				return 0;
			}
			res /= divisor;
			if (!test_int(res)) {
				state = STATE_OVERFLOW;
				return 0;
			}
			break;
		default:	
			curr_token = token;
			return static_cast<int>(res);
		}
	}

	return 0;
}

int calc::eval_neg_number() {
	int next_token = get_next_token();
	if (next_token == SUB) {
		int res=-eval_number();
		if (state != STATE_OK) return 0;
		if (-number < INT_MIN) {
			state = STATE_OVERFLOW;
			return UNKNOWN;
		}
		return res;
	}
	else if (next_token == NUMBER) {
		int res= static_cast<int>(number);
		if (state != STATE_OK) return 0;
		if (number > INT_MAX) {
			state = STATE_OVERFLOW;
			return UNKNOWN;
		}
		return res;
	}
	else {
		state = STATE_UNEXPECTED_CHAR;
		return 0;
	}
}

int calc::eval_number() {
	if (get_next_token() == NUMBER) {
		return static_cast<int>(number);
	}
	else {
		state = STATE_NUMBER_EXPECTED;
		return 0;
	}
}

TOKEN calc::get_next_token() {
	if (curr_token != UNKNOWN) {
		TOKEN tmp = curr_token;
		curr_token = UNKNOWN;
		return tmp;
	}
	char ch = get_curr_char();
	while (ch == ' ') {
		ch = get_next_char();
	}
	if ((ch >= '0') && (ch <= '9')) {
		long long int n = 0;
		do {
			n = n * 10 + ch - '0';
			if (n-1 > INT_MAX) {
				state = STATE_OVERFLOW;
				return UNKNOWN;
			}
			ch = get_next_char();
		} while ((ch >= '0') && (ch <= '9'));
		number = n;
		return NUMBER;
	}
	else {
		switch (ch) {
		case '\0':
			return EOL;
		case '+':
			get_next_char();
			return ADD;
		case '-':
			get_next_char();
			return SUB;
		case '*':
			get_next_char();
			return MUL;
		case '/':
			get_next_char();
			return DIV;
		default:
			return UNKNOWN;
		}
	}
}
char calc::get_next_char() {
	if (pos != '\0') {
		return *(++pos);
	}
	else {
		state = STATE_UNEXPECTED_STRINGEND;
		return '\0';
	}

}

bool calc::test_int(long long int n) const {
 	return (n >= INT_MIN) && (n <= INT_MAX); 
}



