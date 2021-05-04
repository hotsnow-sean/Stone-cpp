#pragma once

#include "Token.h"
#include <exception>

class ParseException : public std::exception
{
private:
	std::string msg;

public:
	explicit ParseException(const std::string& msg);
	explicit ParseException(Token::c_ptr t);
	ParseException(const std::string& msg, Token::c_ptr t);

public:
	const char* what() const noexcept override;
};

