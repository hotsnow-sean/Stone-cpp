#include "ASTNodeType.h"

int NumberLiteral::value() const {
	return token()->getNumber();
}

SObject::ptr NumberLiteral::eval(Environment& env) const {
	return SObject::createInterger(value());
}

std::string Name::name() const {
	return token()->getText();
}

SObject::ptr Name::eval(Environment& env) const {
	auto value = env.get(name());
	if (value == SObject::getNull()) throw StoneException("undefined name: " + name());
	return value;
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

SObject::ptr BinaryExpr::eval(Environment& env) const {
	std::string o = op();
	auto r = right()->eval(env);
	if (o == "=") {
		auto n = std::dynamic_pointer_cast<const Name>(left());
		if (!n) throw StoneException("bad assignment");
		env.put(n->name(), r);
		return r;
	} else {
		auto l = left()->eval(env);
		if (o == "+") return l->__add__(r);
		if (o == "-") return l->__sub__(r);
		if (o == "*") return l->__mul__(r);
		if (o == "/") return l->__div__(r);
		if (o == "%") return l->__mod__(r);
		if (o == "==") return SObject::createBoolean(l->__eq__(r));
		if (o == ">") return SObject::createBoolean(l->__gt__(r));
		if (o == "<") return SObject::createBoolean(l->__lt__(r));
		throw StoneException("bad operator");
	}
}

ASTree::c_ptr NegativeExpr::oprand() const {
	return child(0);
}

std::string NegativeExpr::toString() const {
	return "-" + oprand()->toString();
}

SObject::ptr NegativeExpr::eval(Environment& env) const {
	auto v = oprand()->eval(env);
	return SObject::createInterger(-(SObject::getNumber(v)));
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

SObject::ptr IfStmnt::eval(Environment& env) const {
	bool c = condition()->eval(env)->__bool__();
	if (c) return thenBlock()->eval(env);
	auto b = elseBlock();
	if (!b) return SObject::getNull();
	return b->eval(env);
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

SObject::ptr WhileStmnt::eval(Environment& env) const {
	SObject::ptr ret;
	while (condition()->eval(env)->__bool__()) ret = body()->eval(env);
	if (ret) return ret;
	return SObject::getNull();
}

std::string StringLiteral::value() const {
	return token()->getText();
}

SObject::ptr StringLiteral::eval(Environment& env) const {
	return SObject::createString(value());
}

SObject::ptr BlockStmnt::eval(Environment& env) const {
	SObject::ptr ret;
	int num = numChildren();
	for (int i = 0; i < num; i++) {
		ret = child(i)->eval(env);
	}
	return ret;
}
