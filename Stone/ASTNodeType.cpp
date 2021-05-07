#include "ASTNodeType.h"
#include "BasicType.h"
#include "StoneException.h"
#include "NestedEnv.h"

int NumberLiteral::value() const {
	return token()->getNumber();
}

SObject::ptr NumberLiteral::eval(Environment& env) const {
	return std::make_shared<Integer>(value());
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
		auto p = std::dynamic_pointer_cast<const PrimaryExpr>(left());
		if (p && p->hasPostfix(0)) {
			auto member = std::dynamic_pointer_cast<const Dot>(p->postfix(0));
			if (member) { // 若链式调用的最后是点成员变量运算
				auto obj = std::dynamic_pointer_cast<StoneObject>(p->evalSubExpr(env, 1));
				if (obj) { // 确保运算调用者是实例对象
					try {
						obj->write(member->name(), r); // 为对象的成员赋值
						return r;
					} catch (const std::exception&) {
						throw StoneException("bad member access " + location() + ": " + member->name());
					}
				}
			}
			auto aref = std::dynamic_pointer_cast<const ArrayRef>(p->postfix(0));
			if (aref) { // 若链式调用的最后是数组方括号运算
				auto arr = std::dynamic_pointer_cast<Array>(p->evalSubExpr(env, 1));
				if (arr) { // 确保运算调用者是数组对象
					auto id = std::dynamic_pointer_cast<Integer>(aref->index()->eval(env));
					if (id) {
						(*arr)[id->value()] = r;
						return r;
					}
				}
				throw StoneException("bad array access " + location());
			}
		}
		// 普通变量的赋值操作
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
		if (o == "==") return std::make_shared<Boolean>(l->__eq__(r));
		if (o == ">") return std::make_shared<Boolean>(l->__gt__(r));
		if (o == "<") return std::make_shared<Boolean>(l->__lt__(r));
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
	if (v) return std::make_shared<Integer>(v->value());
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
	return std::make_shared<String>(value());
}

SObject::ptr BlockStmnt::eval(Environment& env) const {
	SObject::ptr ret;
	for (auto t : m_children) ret = t->eval(env);
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
	env.putNew(name(), std::make_shared<Function>(parameters(), body(), &env));
	return std::make_shared<String>(name());
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
	int num = 0;
	for (auto t : m_children) params->eval(newEnv, num++, t->eval(env));
	auto ret = func->body()->eval(*newEnv);
	delete newEnv; // 返回运行结果前销毁函数上下文
	return ret;
}

ASTree::c_ptr PrimaryExpr::operand() const {
	return child(0);
}

ASTree::c_ptr PrimaryExpr::postfix(int nest) const {
	return child((size_t)numChildren() - nest - 1);
}

bool PrimaryExpr::hasPostfix(int nest) const {
	return numChildren() - nest > 1;
}

SObject::ptr PrimaryExpr::eval(Environment& env) const {
	return evalSubExpr(env, 0);
}

SObject::ptr PrimaryExpr::evalSubExpr(Environment& env, int nest) const {
	if (hasPostfix(nest)) {
		auto target = evalSubExpr(env, nest + 1);
		auto p = postfix(nest);
		auto po = std::dynamic_pointer_cast<const Postfix>(p);
		return po->eval(env, target);
	}
	return operand()->eval(env);
}

std::string ClassStmnt::name() const {
	return child(0)->token()->getText();
}

std::string ClassStmnt::superClass() const {
	if (numChildren() < 3) return "";
	return child(1)->token()->getText();
}

ASTree::c_ptr ClassStmnt::body() const {
	return child((size_t)numChildren() - 1);
}

std::string ClassStmnt::toString() const {
	std::string parent = superClass();
	if (parent.empty()) parent += "*";
	return "(class " + name() + " " + parent + " " + body()->toString() + ")";
}

SObject::ptr ClassStmnt::eval(Environment& env) const {
	env.put(name(), std::make_shared<ClassInfo>(this->shared_from_this(), &env));
	return std::make_shared<String>(name());
}

std::string Dot::name() const {
	return child(0)->token()->getText();
}

std::string Dot::toString() const {
	return "." + name();
}

SObject::ptr Dot::eval(Environment& env, SObject::ptr value) const {
	std::string member = name();
	auto ci = std::dynamic_pointer_cast<ClassInfo>(value);
	if (ci && member == "new") {
		auto e = new NestedEnv(ci->environment());
		auto so = std::make_shared<StoneObject>(e);
		initObject(ci, e);
		return so;
	}
	auto so = std::dynamic_pointer_cast<StoneObject>(value);
	if (so) {
		try {
			return so->read(member);
		} catch (const std::exception&) {}
	}
	throw StoneException("bad member access: " + member + " " + location());
}

void Dot::initObject(SObject::ptr ci, Environment* env) const {
	auto c = std::dynamic_pointer_cast<ClassInfo>(ci);
	auto p = c->superClass();
	if (p) initObject(p, env);
	c->body()->eval(*env);
}

SObject::ptr ClassBody::eval(Environment& env) const {
	for (auto t : m_children) t->eval(env);
	return nullptr;
}

int ArrayLiteral::size() const noexcept {
	return numChildren();
}

SObject::ptr ArrayLiteral::eval(Environment& env) const {
	auto res = std::make_shared<Array>(m_children.size());
	size_t i = 0;
	for (auto t : m_children) {
		(*res)[i++] = t->eval(env);
	}
	return res;
}

ASTree::c_ptr ArrayRef::index() const {
	return child(0);
}

std::string ArrayRef::toString() const {
	return "[" + index()->toString() + "]";
}

SObject::ptr ArrayRef::eval(Environment& env, SObject::ptr value) const {
	auto arr = std::dynamic_pointer_cast<Array>(value);
	if (arr) {
		auto id = std::dynamic_pointer_cast<Integer>(index()->eval(env));
		if (id) return (*arr)[id->value()];
	}
	throw StoneException("bad array access " + location());
}
