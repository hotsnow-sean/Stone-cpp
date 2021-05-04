#include "ASTNodeType.h"

int NumberLiteral::value() const {
	return token()->getNumber();
}

std::string Name::name() const {
	return token()->getText();
}

ASTree::c_ptr BinaryExpr::left() const {
	return child(0);
}

ASTree::c_ptr BinaryExpr::right() const {
	return child(2);
}

std::string BinaryExpr::op() const {
	return child(1)->token()->getText();
}

ASTree::c_ptr NegativeExpr::oprand() const {
	return child(0);
}

std::string NegativeExpr::toString() const {
	return "-" + oprand()->toString();
}

ASTree::c_ptr IfStmnt::condition() const {
	return child(0);
}

ASTree::c_ptr IfStmnt::thenBlock() const {
	return child(1);
}

ASTree::c_ptr IfStmnt::elseBlock() const {
	return numChildren() > 2 ? child(2) : nullptr;
}

std::string IfStmnt::toString() const {
	std::string ret = "(if " + condition()->toString() + " " + thenBlock()->toString();
	auto el = elseBlock();
	if (el) ret += " else " + el->toString();
	ret += ")";
	return ret;
}

ASTree::c_ptr WhileStmnt::condition() const {
	return child(0);
}

ASTree::c_ptr WhileStmnt::body() const {
	return child(1);
}

std::string WhileStmnt::toString() const {
	return "(while " + condition()->toString() + " " + body()->toString() + ")";
}

std::string StringLiteral::value() const {
	return token()->getText();
}
