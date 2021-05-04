#pragma once

// Token 词法分析器将输入分割为 token，利用此类来保存每一个 token

#include <string>
#include <memory>

// Token 基类，可操作三类 token，分别为 [整型数值、标识符、字符串]
// 标识符与字符串的区别是字符串由双引号包裹
// 由于类型不单一，具体存储方式由子类去实现，基类提供访问模板
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
	virtual std::string getText() const noexcept; // 所有类型的 token 都可以输出为字符串
};

