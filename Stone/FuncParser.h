#pragma once

#include "BasicParse.h"

class FuncParser : public BasicParse
{
protected:
	ListRule<>* param;
	ListRule<ParameterList>* params;
	ListRule<>* paramList;
	ListRule<DefStmnt>* def;
	ListRule<Arguments>* args;
	ListRule<>* postfix;

public:
	FuncParser(Lexer& l);
};

