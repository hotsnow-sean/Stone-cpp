#pragma once

#include "FuncParser.h"

// ����������̳��� Func��Ҳ���ǲ��߱����������
// ��ͬʱ��Ҫ��������Ĺ��ܣ����Դ���һ����̳е�����
// ������Ҫ����������ļ̳й�ϵ��Ϊ��̳У����������μ̳е�ȱ��

class ArrayParser : public FuncParser
{
protected:
	ListRule<ArrayLiteral>* elements;

public:
	ArrayParser(Lexer& l);
};

