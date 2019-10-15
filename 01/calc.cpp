#include <iostream>
#include <climits>
#include <string.h>

#define TESTS_BEGIN \
	int N_TESTS = 0; \
	int N_FAILED_TESTS = 0; 

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
} ;

const char* messages[] = {
	"OK",
	"Expression is nullptr",
	"Unexpected end of expression",
	"Unexpected character",
	"int type îverflow",
	"Number expected",
	"Division by zero",
};

class calc 
{
public:	
	int eval(const char* s);
	STATE get_state() const { return state; };

private:
	STATE state=STATE_OK;
	char* pos=nullptr;
	long long int number = 0;
	int curr_token = UNKNOWN;

	int eval_sum_sub();
	int eval_mul_div();
	int eval_neg_number();
	int eval_number();

	int get_next_token();
	char get_next_char();
	char get_curr_char() const { return *pos; }

	bool TEST_int(long long int n) { return (n >= INT_MIN) && (n <= INT_MAX); }
};

int calc::eval(const char* s) {	
	pos = const_cast<char*>(s);
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
			if (!TEST_int(res)) {
				state = STATE_OVERFLOW;
				return 0;
			}
			break;
		case SUB:
			res -= eval_mul_div();			
			if (!TEST_int(res)) {
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
		switch (int token=get_next_token()) {
		case MUL:			
			res *= eval_neg_number();			
			if (!TEST_int(res)) {
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
			if (!TEST_int(res)) {
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

int calc::get_next_token() {
	if (curr_token != UNKNOWN) {
		int tmp = curr_token;
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
void test() {
	TESTS_BEGIN
	calc c;
	//correct expressions
	TEST((c.eval("2*2") == 4) && (c.get_state() == STATE_OK))
	TEST((c.eval("2+2") == 4) && (c.get_state() == STATE_OK))
	TEST((c.eval("2-2") == 0) && (c.get_state() == STATE_OK))
	TEST((c.eval("-2147483648") == INT_MIN) && (c.get_state() == STATE_OK))
	TEST((c.eval("2147483647") == INT_MAX) && (c.get_state() == STATE_OK))
	TEST((c.eval(" 1  +  2+3  + 4   +   5+6+7+8 +  9+10") == 55) && (c.get_state() == STATE_OK))
	TEST((c.eval("-1  -  2-3  - 4   -   5-6-7-8 -  9-10") ==-55) && (c.get_state() == STATE_OK))
	TEST((c.eval("1 * 2*3*4*5   *   6*7*8 *9*   10  ") == 3628800) && (c.get_state() == STATE_OK))
	TEST((c.eval("255 /- 2") == -127) && (c.get_state() == STATE_OK))
	TEST((c.eval("255/15") == 17) && (c.get_state() == STATE_OK))
	//12345679*8==98765432
	TEST((c.eval(" 12345679   * 8 ") == 98765432) && (c.get_state() == STATE_OK))
	TEST((c.eval(" 256  * 256 * 256 ") == 16777216) && (c.get_state() == STATE_OK))
	TEST((c.eval(" - 256  * - 256 * -256 ") == -16777216) && (c.get_state() == STATE_OK))
	//division by zero 
	TEST((c.eval(" 1+2*3/4-5*6+7*9/0 ") == 0) && (c.get_state() == STATE_DIVISION_BY_ZERO))
	//overflow
	TEST((c.eval(" 256 * 256 * 256 * 256 ") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST((c.eval("-2147483647 - 1 -1") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST((c.eval("-2147483648/ - 1") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST((c.eval("-2147483648 * - 1") == 0) && (c.get_state() == STATE_OVERFLOW))
	TEST((c.eval("2147483648") == 0) && (c.get_state() == STATE_OVERFLOW))
	//2 unary minus not allowed
	TEST((c.eval(" - 256  * - 256 * -  -  256 ") == 0) && (c.get_state() == STATE_NUMBER_EXPECTED))
	//unary plus not allowed
	TEST((c.eval(" - 256  * - 256 * + 256 ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval(" -256 - + 256 ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	//INT_MIN
	TEST((c.eval("-2147483647 - 1") == INT_MIN) && (c.get_state() == STATE_OK))
	//incorrect expressions
	TEST((c.eval(nullptr) == 0) && (c.get_state() == STATE_NULLPTR))
	TEST((c.eval("") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("         ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  -   ") == 0) && (c.get_state() == STATE_NUMBER_EXPECTED))
	TEST((c.eval("  +   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  *   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  /   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  1 1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  1.1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  1**1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  1//1   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TEST((c.eval("  a   ") == 0) && (c.get_state() == STATE_UNEXPECTED_CHAR))
	TESTS_END
}
int main(int argc,char* argv[]) {
	if (argc > 1) {
		if (strcmp("test",argv[1])==0) {
			test();
			return 0;
		}
		else 
		{
			calc c;
			int res = c.eval(argv[1]);
			int state = c.get_state();
			if (STATE_OK == state) {
				std::cout << res << std::endl;
			}
			else {
				std::cout << "Error: " << messages[state] << " in expression: " << argv[1] << std::endl;
			}
			return state;
		}
	}
	else {
		std::cout << "Usage: calc <expression>|test" << std::endl;

		return 0;
	}
}

