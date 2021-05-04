#pragma once

// Token �ʷ�������������ָ�Ϊ token�����ô���������ÿһ�� token

#include <string>
#include <memory>

// Token ���࣬�ɲ������� token���ֱ�Ϊ [������ֵ����ʶ�����ַ���]
// ��ʶ�����ַ������������ַ�����˫���Ű���
// �������Ͳ���һ������洢��ʽ������ȥʵ�֣������ṩ����ģ��
class Token
{
public:
	typedef std::shared_ptr<Token> ptr;
	typedef std::shared_ptr<const Token> c_ptr;

	static const Token::c_ptr eof; // end of file
	static const std::string eol;  // end of line

private:
	int line_number;

public:
	explicit Token(int line);
	virtual ~Token() = default;

public:
	int getLineNumber() const noexcept;

	virtual bool isIdentifier() const noexcept;
	virtual bool isNumber() const noexcept;
	virtual bool isString() const noexcept;

	virtual int getNumber() const;
	virtual std::string getText() const noexcept; // �������͵� token ���������Ϊ�ַ���
};

