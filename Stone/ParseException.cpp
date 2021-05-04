#include "ParseException.h"

static std::string location(Token::c_ptr t) {
	if (t == Token::eof) return "the last line";
	return "'" + t->getText() + "' at line " + std::to_string(t->getLineNumber());
}

ParseException::ParseException(const std::string& msg) : msg(msg) {}

ParseException::ParseException(Token::c_ptr t) : ParseException("", t) {}

ParseException::ParseException(const std::string& msg, Token::c_ptr t)
	: ParseException("syntax error around " + location(t) + ". " + msg) {}

const char* ParseException::what() const noexcept {
	return msg.c_str();
}
