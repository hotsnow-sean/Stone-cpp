#pragma once

// Lexer �ʷ��������������������

#include "Token.h"
#include <regex>
#include <deque>
#include <istream>

class Lexer
{
private:
	std::deque<Token::c_ptr> queue;	// �洢���ɵ� token

	std::istream& reader; // ���ⲿ���룬���ڶ�ȡҪ�ָ����Ϣ

	bool has_more{ true }; // �Ƿ���ʣ��ɷ����������Ƿ��ȡ�� EOF ����
	int line_number{ 0 };  // �к�

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

