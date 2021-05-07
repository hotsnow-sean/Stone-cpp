#pragma once

#include "BasicParse.h"

// 在基础语法解析器的基础上，对规则加以扩充与简单的修改，达成解析函数的目的

class FuncParser : public BasicParse
{
protected:
	ListRule<>* param;
	ListRule<ParameterList>* params;
	ListRule<>* paramList;
	ListRule<DefStmnt>* def;
	ListRule<Arguments>* args;
	OrRule* postfix;

public:
	FuncParser(Lexer& l);
};

