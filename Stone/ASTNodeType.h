#pragma once

#include "ASTree.h"

class NumberLiteral : public ASTLeaf {
public:
	using ASTLeaf::ASTLeaf;

	int value() const;
};

class Name : public ASTLeaf {
public:
	using ASTLeaf::ASTLeaf;

	std::string name() const;
};

class StringLiteral : public ASTLeaf {
public:
	using ASTLeaf::ASTLeaf;

	std::string value() const;
};

class BinaryExpr : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr left() const;
	ASTree::c_ptr right() const;
	std::string op() const;
};

class NegativeExpr : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr oprand() const;
	std::string toString() const final;
};

class BlockStmnt : public ASTList {
public:
	using ASTList::ASTList;
};

class IfStmnt : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr condition() const;
	ASTree::c_ptr thenBlock() const;
	ASTree::c_ptr elseBlock() const;
	std::string toString() const final;
};

class WhileStmnt : public ASTList {
public:
	using ASTList::ASTList;

	ASTree::c_ptr condition() const;
	ASTree::c_ptr body() const;
	std::string toString() const final;
};