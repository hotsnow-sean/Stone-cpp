#pragma once

// Lexer 词法分析器，利用正则解析

#include "Token.h"
#include <regex>
#include <deque>
#include <istream>

class Lexer
{
private:
	std::deque<Token::c_ptr> queue;	// 存储生成的 token

	std::istream& reader; // 由外部传入，用于读取要分割的信息

	bool has_more{ true }; // 是否还有剩余可分析，根据是否读取到 EOF 设置
	int line_number{ 0 };  // 行号

public:
	Lexer(std::istream& reader);

public:
	Token::c_ptr read();
	Token::c_ptr peek(size_t i);

private:
	bool fillQueue(size_t i);
	void readLine();
	void addToken(int lineNo, const std::smatch& matcher);
};

