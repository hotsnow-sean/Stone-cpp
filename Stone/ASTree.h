#pragma once

// ASTree �﷨���洢��ʽ���洢�ս�������ӽڵ�����

#include "Token.h"
#include "Environment.h"
#include <vector>

class ASTree
{
public:
	typedef std::shared_ptr<ASTree> ptr;
	typedef std::shared_ptr<const ASTree> c_ptr;

	virtual ~ASTree() = default;

	virtual bool isLeaf() const = 0;
	virtual int numChildren() const = 0;
	virtual Token::c_ptr token() const = 0;
	virtual ASTree::c_ptr child(size_t i) const = 0;
	virtual std::string location() const = 0;
	virtual std::string toString() const = 0;
	virtual SObject::ptr eval(Environment& env) const = 0;
};

class ASTLeaf : public ASTree {
protected:
	Token::c_ptr m_token;

public:
	ASTLeaf(Token::c_ptr t);

	// ͨ�� ASTree �̳�
	bool isLeaf() const noexcept final;
	int numChildren() const noexcept final;
	Token::c_ptr token() const noexcept final;
	ASTree::c_ptr child(size_t i) const final;
	virtual std::string location() const noexcept override;
	virtual std::string toString() const noexcept override;
	virtual SObject::ptr eval(Environment& env) const override;
};

class ASTList : public ASTree {
protected:
	std::vector<ASTree::c_ptr> m_children;

public:
	ASTList(const std::vector<ASTree::c_ptr>& list);

	// ͨ�� ASTree �̳�
	virtual bool isLeaf() const noexcept override;
	virtual int numChildren() const noexcept override;
	virtual Token::c_ptr token() const override;
	virtual ASTree::c_ptr child(size_t i) const override;
	virtual std::string location() const override;
	virtual std::string toString() const override;
	virtual SObject::ptr eval(Environment& env) const override;
};

