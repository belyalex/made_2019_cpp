#include <stdexcept>
#include "tokenizer.h"

Tokenizer::Tokenizer(OnParseBegin* onParseBegin, OnParseEnd* onParseEnd, OnIntToken* onIntToken, OnStringToken* onStringToken) :
	onParseBegin(onParseBegin),
	onParseEnd(onParseEnd),
	onIntToken(onIntToken),
	onStringToken(onStringToken) 
{
}

void Tokenizer::tokenize(const std::string& str) {
	if (onParseBegin != nullptr) {
		onParseBegin();
	}

	bool is_numeric = true;
	std::string token;
	for (int i = 0; i <= str.length(); i++) {
		switch (str[i]) {
		case ' ':
		case '\t':
		case '\n':
		case '\0':
			if (token.length() != 0) {
				if (is_numeric) {
					try {
						int result = std::stoi(token);
						if (onIntToken != nullptr) {
							onIntToken(result);
						}
						token = "";
						break;
					} 
					catch (std::out_of_range& e) {
						//если не смогли преобразовать в число - ничего не делаем, дальше вызовется callback для строки
					}
				}
				if (onStringToken != nullptr) {
					onStringToken(token);
				}				
			}
			token = "";
			is_numeric = true;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			token += str[i];
			break;
		default:
			token += str[i];
			is_numeric = false;
		}
	}

	if (onParseEnd != nullptr) {
		onParseEnd();
	}
}
