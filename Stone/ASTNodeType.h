#pragma once

#include "ASTree.h"

class NumberLiteral : public ASTLeaf {
public:
	using ASTLeaf::ASTLeaf;

	int value() const;

	virtual SObject::ptr eval(Environment& env) const override;
};

class Name : public ASTLeaf {
public:
	using ASTLeaf::ASTLeaf;

	std::string name() const;

	virtual SObject::ptr eval(Environment& env) const override;
};

class StringLiteral : public ASTLeaf {
public:
	using ASTLeaf::ASTLeaf;

	std::string value() const;

	virtual SObject::ptr eval(Environment& env) const override;
};

class PrimaryExpr : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr operand() const;
	ASTree::c_ptr postfix(int nest) const;

	bool hasPostfix(int nest) const;

	virtual SObject::ptr eval(Environment& env) const override;
	SObject::ptr evalSubExpr(Environment& env, int nest) const;
};

class BinaryExpr : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr left() const;
	ASTree::c_ptr right() const;
	std::string op() const;
	virtual SObject::ptr eval(Environment& env) const override;
};

class NegativeExpr : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr oprand() const;
	std::string toString() const final;
	virtual SObject::ptr eval(Environment& env) const override;
};

class BlockStmnt : public ASTList {
public:
	using ASTList::ASTList;

	virtual SObject::ptr eval(Environment& env) const override;
};

class IfStmnt : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr condition() const;
	ASTree::c_ptr thenBlock() const;
	ASTree::c_ptr elseBlock() const;
	std::string toString() const final;
	virtual SObject::ptr eval(Environment& env) const override;
};

class WhileStmnt : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr condition() const;
	ASTree::c_ptr body() const;
	std::string toString() const final;
	virtual SObject::ptr eval(Environment& env) const override;
};

class NullStmnt : public ASTList {
public:
	using ASTList::ASTList;
};

class ParameterList : public ASTList {
public:
	using ASTList::ASTList;

	std::string name(int i) const;
	int size() const noexcept;

	void eval(Environment* env, int index, SObject::ptr value) const;
};

class DefStmnt : public ASTList {
public:
	using ASTList::ASTList;

	std::string name() const;
	ASTree::c_ptr parameters() const;
	ASTree::c_ptr body() const;
	std::string toString() const final;
	virtual SObject::ptr eval(Environment& env) const override;
};

class Postfix : public ASTList {
public:
	using ASTList::ASTList;

	virtual SObject::ptr eval(Environment& env, SObject::ptr value) const = 0;
};

class Arguments : public Postfix, public std::enable_shared_from_this<Arguments> {
public:
	using Postfix::Postfix;

	int size() const noexcept;

	SObject::ptr eval(Environment& env, SObject::ptr value) const final;
};

class ClassBody : public ASTList {
public:
	using ASTList::ASTList;

	virtual SObject::ptr eval(Environment& env) const override;
};

class ClassStmnt : public ASTList, public std::enable_shared_from_this<ClassStmnt> {
public:
	using ASTList::ASTList;

	std::string name() const;
	std::string superClass() const;
	ASTree::c_ptr body() const;
	std::string toString() const final;

	virtual SObject::ptr eval(Environment& env) const override;
};

class Dot : public Postfix {
public:
	using Postfix::Postfix;

	std::string name() const;
	std::string toString() const final;

	// Í¨¹ý Postfix ¼Ì³Ð
	virtual SObject::ptr eval(Environment& env, SObject::ptr value) const override;

protected:
	void initObject(SObject::ptr ci, Environment* env) const;
};