#pragma once

// �������� Parse �������ɵ� Stone ���Զ�Ӧ���﷨������
// ���� parse ���������������ľ���Ϊ��Ԫ����Դ�ļ�

#include "ASTNodeType.h"
#include "Parse.hpp"

// �������﷨������
class BasicParse
{
protected:
	Lexer& m_lexer;
	ParseFactory m_factory;

	ListRule<>* program;	// [ statement ] (";" | EOL)
	OrRule* statement;		// "if" expr block [ "else" block ] | "while" expr block | simple
	ListRule<PaimaryExpr>* simple;		// expr
	ListRule<BlockStmnt>* block; // "{" [ statement ] { (";" | EOL) [ statement ] } "}"
	ListRule<>* expr;		// factor { OP factor }
	OrRule* factor;			// "-" primary | primary
	ListRule<PaimaryExpr>* primary;	// "(" expr ")" | NUMBER | IDENTIFIER | STRING

public:
	BasicParse(Lexer& l);

	ASTree::c_ptr parse();
};

