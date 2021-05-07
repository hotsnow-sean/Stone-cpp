#pragma once

#include "FuncParser.h"

// 数组分析器继承于 Func，也就是不具备类分析功能
// 如同时需要数组与类的功能，可以创建一个多继承的新类
// 不过需要将数组与类的继承关系改为虚继承，来消除菱形继承的缺点

class ArrayParser : public FuncParser
{
protected:
	ListRule<ArrayLiteral>* elements;

public:
	ArrayParser(Lexer& l);
};

