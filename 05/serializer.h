#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <string>


enum class Error
{
	NoError,
	CorruptedArchive
};

namespace made {

class Serializer
{
private:	
	std::ostream& out;	
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out)
		: out(out)
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}
private:
	template<class T>
	Error save_value(T val) {
		return ((out << val << Separator) ? Error::NoError : Error::CorruptedArchive);
	}
	
	template<class T>
	Error process(T&& val)
	{
		return save_value(val);		
	}

	template<class T, class... ArgsT>
	Error process(T&& val, ArgsT&&... args)
	{
		Error res = save_value(val);
		if (res != Error::NoError) return res;
		return process(std::forward<ArgsT>(args)...);
	}
};

class Deserializer
{
	std::istream& in;
	static constexpr char Separator = ' ';
public:
	explicit Deserializer(std::istream& in)
		: in(in)
	{
	}

	template <class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}

private:
	template<class T>
	Error load_value(T& val) {
		if (in >> val)
			return Error::NoError;
		else
			return Error::CorruptedArchive;
	};

	template<class T>
	Error process(T&& val)
	{
		return load_value<T&&>(val);
	}
	template<class T, class... ArgsT>
	Error process(T&& val, ArgsT&&... args)
	{
		Error res=load_value<T&&>(val);

		if (res != Error::NoError) return res;

		return process(std::forward<ArgsT>(args)...);
	}	
	
};

template<>
Error Serializer::save_value<bool>(bool val)
{
	return ((out << (val ? "true" : "false") << Separator)? Error::NoError : Error::CorruptedArchive);		
}

template<>
Error Deserializer::load_value<bool&>(bool& val)
{
	std::string text;
	in >> text;
	if (text == "true")
		val = true;
	else if (text == "false")
		val = false;
	else
		return Error::CorruptedArchive;

	return Error::NoError;
	}
}

#endif
