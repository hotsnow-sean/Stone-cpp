#pragma once

// 这是利用 Parse 工具生成的 Stone 语言对应的语法解析器
// 调用 parse 方法即可以完整的句子为单元解析源文件

#include "ASTNodeType.h"
#include "Parse.hpp"
#include <unordered_set>

// 基本的语法解析器
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

	ListRule<NullStmnt>* nulltemp; // 空语句解析器，即不包含任何逻辑的规则
	OrRule* primaryOr; // 辅助规则
	OrRule* programOr;

public:
	BasicParse(Lexer& l);

	ASTree::c_ptr parse();
};

