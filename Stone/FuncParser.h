#pragma once

#include "BasicParse.h"

// �ڻ����﷨�������Ļ����ϣ��Թ������������򵥵��޸ģ���ɽ���������Ŀ��

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

