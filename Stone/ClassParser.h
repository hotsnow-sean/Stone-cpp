#pragma once

#include "FuncParser.h"

class ClassParser : public FuncParser
{
protected:
	OrRule* member;
	ListRule<ClassBody>* classBody;
	ListRule<ClassStmnt>* defclass;

public:
	ClassParser(Lexer& l);
};

