#include "ASTNodeType.h"
#include "BasicType.h"
#include "StoneException.h"

int NumberLiteral::value() const {
	return token()->getNumber();
}

SObject::ptr NumberLiteral::eval(Environment& env) const {
	return SObject::ptr(new Integer(value()));
}

std::string Name::name() const {
	return token()->getText();
}

SObject::ptr Name::eval(Environment& env) const {
	auto value = env.get(name());
	if (value == nullptr) throw StoneException("undefined name: " + name());
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
		if (o == "==") return SObject::ptr(new Boolean(l->__eq__(r)));
		if (o == ">") return SObject::ptr(new Boolean(l->__gt__(r)));
		if (o == "<") return SObject::ptr(new Boolean(l->__lt__(r)));
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
	auto v = std::dynamic_pointer_cast<Integer>(oprand()->eval(env));
	if (v) return SObject::ptr(new Integer(v->value()));
	throw StoneException("bad type for '-'");
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
	if (!b) return nullptr;
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
	return nullptr;
}

std::string StringLiteral::value() const {
	return token()->getText();
}

SObject::ptr StringLiteral::eval(Environment& env) const {
	return SObject::ptr(new String(value()));
}

SObject::ptr BlockStmnt::eval(Environment& env) const {
	SObject::ptr ret;
	int num = numChildren();
	for (int i = 0; i < num; i++) {
		ret = child(i)->eval(env);
	}
	return ret;
}

std::string ParameterList::name(int i) const {
	return child(i)->token()->getText();
}

int ParameterList::size() const noexcept {
	return numChildren();
}

void ParameterList::eval(Environment* env, int index, SObject::ptr value) const {
	env->putNew(name(index), value);
}

std::string DefStmnt::name() const {
	return child(0)->token()->getText();
}

ASTree::c_ptr DefStmnt::parameters() const {
	return child(1);
}

ASTree::c_ptr DefStmnt::body() const {
	return child(2);
}

std::string DefStmnt::toString() const {
	return "(def " + name() + " " + parameters()->toString() + " " + body()->toString() + ")";
}

SObject::ptr DefStmnt::eval(Environment& env) const {
	env.putNew(name(), SObject::ptr(new Function(parameters(), body(), &env)));
	return SObject::ptr(new String(name()));
}

int Arguments::size() const noexcept {
	return numChildren();
}

SObject::ptr Arguments::eval(Environment& env, SObject::ptr value) const {
	auto native = std::dynamic_pointer_cast<NativeFunction>(value);
	if (native) {
		int nparams = native->numOfParameters();
		if (nparams != -1 && size() != nparams) throw StoneException("bad number of arguments " + this->location());
		int sum = numChildren();
		std::vector<SObject::ptr> args(sum);
		for (int i = 0; i < sum; i++) {
			args[i] = child(i)->eval(env);
		}
		return native->invoke(args, this->shared_from_this());
	}
	auto func = std::dynamic_pointer_cast<Function>(value);
	if (!func) throw StoneException("bad function " + this->location());
	auto params = std::dynamic_pointer_cast<const ParameterList>(func->parameters());
	if (!params) throw StoneException("bad parameters " + this->location());
	if (size() != params->size()) throw StoneException("bas number of arguments " + this->location());
	Environment* newEnv = func->makeEnv(); // 创建一个临时的函数计算环境
	int num = 0, sum = numChildren();
	for (int i = 0; i < sum; i++) {
		params->eval(newEnv, num++, child(i)->eval(env));
	}
	auto ret = func->body()->eval(*newEnv);
	delete newEnv; // 返回运行结果前销毁函数上下文
	return ret;
}

ASTree::c_ptr PaimaryExpr::operand() const {
	return child(0);
}

ASTree::c_ptr PaimaryExpr::postfix(int nest) const {
	return child(numChildren() - nest - 1);
}

bool PaimaryExpr::hasPostfix(int nest) const {
	return numChildren() - nest > 1;
}

SObject::ptr PaimaryExpr::eval(Environment& env) const {
	return evalSubExpr(env, 0);
}

SObject::ptr PaimaryExpr::evalSubExpr(Environment& env, int nest) const {
	if (hasPostfix(nest)) {
		auto target = evalSubExpr(env, nest + 1);
		auto p = postfix(nest);
		auto po = std::dynamic_pointer_cast<const Postfix>(p);
		return po->eval(env, target);
	}
	return operand()->eval(env);
}
