#pragma once

// 这是利用 Parse 工具生成的 Stone 语言对应的语法解析器
// 调用 parse 方法即可以完整的句子为单元解析源文件

#include "Parse.hpp"

// 基本的语法解析器
class BasicParse
{
private:
	Lexer& m_lexer;
	ParseFactory m_factory;
	Rule* program;

public:
	BasicParse(Lexer& l);

	ASTree::c_ptr parse();
};

