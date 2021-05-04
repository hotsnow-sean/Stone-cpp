#pragma once

// �������� Parse �������ɵ� Stone ���Զ�Ӧ���﷨������
// ���� parse ���������������ľ���Ϊ��Ԫ����Դ�ļ�

#include "Parse.hpp"

// �������﷨������
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

