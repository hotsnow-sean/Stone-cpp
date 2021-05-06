#pragma once

// 这是利用 Parse 工具生成的 Stone 语言对应的语法解析器
// 调用 parse 方法即可以完整的句子为单元解析源文件

#include "ASTNodeType.h"
#include "Parse.hpp"

// 基本的语法解析器
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

