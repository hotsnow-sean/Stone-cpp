#include "Token.h"
#include "StoneException.h"

const Token::c_ptr Token::eof = std::make_shared<const Token>(-1);
const std::string Token::eol("\\n");

Token::Token(int line) : line_number(line) {}

int Token::getLineNumber() const noexcept {
	return line_number;
}

bool Token::isIdentifier() const noexcept {
	return false;
}

bool Token::isNumber() const noexcept {
	return false;
}

bool Token::isString() const noexcept {
	return false;
}

// �������Ͳ���Ӧ��������� throw StoneException �쳣
int Token::getNumber() const {
	throw StoneException("not number token");
}

std::string Token::getText() const noexcept {
	return "";
}
