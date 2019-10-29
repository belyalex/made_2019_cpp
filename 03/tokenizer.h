#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

using OnParseBegin = void ();
using OnParseEnd = void ();
using OnIntToken = void (const int value);
using OnStringToken = void (const std::string& value);

class Tokenizer {
public:
	Tokenizer(OnParseBegin* onParseBegin, OnParseEnd* onParseEnd, OnIntToken* onIntToken, OnStringToken* onStringToken);
	void tokenize(const std::string& str);
private:
	OnParseBegin*	onParseBegin = nullptr;
	OnParseEnd*		onParseEnd = nullptr;
	OnIntToken*		onIntToken = nullptr;
	OnStringToken*  onStringToken = nullptr;
};

#endif
