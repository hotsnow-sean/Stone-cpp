#pragma once

// �������� Parse �������ɵ� Stone ���Զ�Ӧ���﷨������
// ���� parse ���������������ľ���Ϊ��Ԫ����Դ�ļ�

#include "ASTNodeType.h"
#include "Parse.hpp"
#include <unordered_set>

// �������﷨������
class BasicParse
{
protected:
	static std::unordered_set<std::string> m_reserved;

	Lexer& m_lexer;
	ParseFactory m_factory;

	ListRule<>* program;	// [ statement ] (";" | EOL)
	OrRule* statement;		// "if" expr block [ "else" block ] | "while" expr block | simple
	ListRule<PrimaryExpr>* simple;		// expr
	ListRule<BlockStmnt>* block; // "{" [ statement ] { (";" | EOL) [ statement ] } "}"
	ListRule<>* expr;		// factor { OP factor }
	OrRule* factor;			// "-" primary | primary
	ListRule<PrimaryExpr>* primary;	// "(" expr ")" | NUMBER | IDENTIFIER | STRING

	ListRule<NullStmnt>* nulltemp; // ���������������������κ��߼��Ĺ���
	OrRule* primaryOr; // ��������
	OrRule* programOr;

public:
	BasicParse(Lexer& l);

	ASTree::c_ptr parse();
};

