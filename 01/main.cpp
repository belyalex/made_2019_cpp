#include <iostream>

#include "calc.h"

int main(int argc,char* argv[]) {
	if (argc > 1) {
		calc c;
		int res = c.eval(argv[1]);
		int state = c.get_state();
		if (STATE_OK == state) {
			std::cout << res << std::endl;
		}
		else {
			std::cout << "Error: " << c.get_message() <<" in expression: " << argv[1] << std::endl;
		}
		return state;
		
	}
	else {
		std::cout << "Usage: calc <expression>" << std::endl;

		return 0;
	}
}
