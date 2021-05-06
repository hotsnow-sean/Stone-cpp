#include "BasicType.h"
#include "NestedEnv.h"
#include "StoneException.h"
#include "ASTNodeType.h"
#include <sstream>

// ******************** Integer ********************

const std::shared_ptr<const std::string> Integer::TYPE{ new std::string("Integer") };
Integer::Integer(int v) : SObject(TYPE), m_value(v) {}

bool Integer::__lt__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '<': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return m_value < o->m_value;
}
bool Integer::__gt__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '>': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return m_value > o->m_value;
}
bool Integer::__eq__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '==': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return m_value == o->m_value;
}
bool Integer::__bool__() const noexcept { return m_value != 0; }
SObject::ptr Integer::__add__(SObject::c_ptr other) const {
	if (other->__name__ == String::TYPE) return SObject::ptr(new String(__str__() + other->__str__()));
	if (other->__name__ != TYPE) throw StoneException("error type for '+': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return SObject::ptr(new Integer(m_value + o->m_value));
}
SObject::ptr Integer::__sub__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '-': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return SObject::ptr(new Integer(m_value - o->m_value));
}
SObject::ptr Integer::__mul__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '*': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return SObject::ptr(new Integer(m_value * o->m_value));
}
SObject::ptr Integer::__div__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '/': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return SObject::ptr(new Integer(m_value / o->m_value));
}
SObject::ptr Integer::__mod__(SObject::c_ptr other) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '%': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Integer>(other);
	return SObject::ptr(new Integer(m_value % o->m_value));
}
std::string Integer::__str__() const noexcept { return std::to_string(m_value); }

int Integer::value() const noexcept { return m_value; }

// ******************** String ********************

const std::shared_ptr<const std::string> String::TYPE{ new std::string("String") };
String::String(const std::string& str) : SObject(TYPE), m_value(str) {}

bool String::__eq__(SObject::c_ptr other = false) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '==': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const String>(other);
	return m_value == o->value();
}
bool String::__bool__() const noexcept { return m_value.size(); }
SObject::ptr String::__add__(SObject::c_ptr other = false) const {
	return SObject::ptr(new String(m_value + other->__str__()));
}
std::string String::__str__() const noexcept { return m_value; }

std::string String::value() const noexcept { return m_value; }

// ******************** Boolean ********************

const std::shared_ptr<const std::string> Boolean::TYPE{ new std::string("Boolean") };
Boolean::Boolean(bool v) : SObject(TYPE), m_value(v) {}

bool Boolean::__eq__(SObject::c_ptr other = false) const {
	if (other->__name__ != TYPE) throw StoneException("error type for '==': " + *(other->__name__));
	auto o = std::dynamic_pointer_cast<const Boolean>(other);
	return m_value == o->value();
}
bool Boolean::__bool__() const noexcept { return m_value; }
std::string Boolean::__str__() const noexcept { return m_value ? "True" : "False"; }

bool Boolean::value() const noexcept { return m_value; }

// ******************** Function ********************

const std::shared_ptr<const std::string> Function::TYPE{ new std::string("Function") };
Function::Function(ASTree::c_ptr p, ASTree::c_ptr body, Environment* env) : SObject(TYPE), m_parameters(p), m_body(body), env(env) {}

ASTree::c_ptr Function::parameters() const { return m_parameters; }
ASTree::c_ptr Function::body() const { return m_body; }
Environment* Function::makeEnv() const { return new NestedEnv(env); }

std::string Function::__str__() const noexcept {
	std::stringstream ss;
	ss << "<fun:" << this << ">";
	return ss.str();
}

// ******************** Function ********************

const std::shared_ptr<const std::string> NativeFunction::TYPE{ new std::string("NativeFunction") };
NativeFunction::NativeFunction(const std::string& name, method_ptr m, int argnum) : SObject(TYPE), name(name), method(m), numParams(argnum) {}

int NativeFunction::numOfParameters() const noexcept {
	return numParams;
}

SObject::ptr NativeFunction::invoke(const std::vector<SObject::ptr>& args, ASTree::c_ptr tree) {
	try {
		return method(args);
	} catch (...) {
		throw StoneException("bad native function call: " + name, tree);
	}
}

std::string NativeFunction::__str__() const noexcept {
	std::stringstream ss;
	ss << "<native:" << this << ">";
	return ss.str();
}

const std::shared_ptr<const std::string> ClassInfo::TYPE{ new std::string("ClassInfo") };
ClassInfo::ClassInfo(ASTree::c_ptr cs, Environment* env) : SObject(TYPE), definition(cs), m_superClass(nullptr), env(env) {
	auto d = std::dynamic_pointer_cast<const ClassStmnt>(definition);
	if (!d) throw StoneException("bad class defination");
	auto obj = env->get(d->superClass());
	if (obj) {
		auto o = std::dynamic_pointer_cast<ClassInfo>(obj);
		if (!o) throw StoneException("unknown super class: " + d->superClass(), d);
		m_superClass = o;
	}
}

std::string ClassInfo::name() const {
	auto d = std::dynamic_pointer_cast<const ClassStmnt>(definition);
	return d->name();
}

SObject::ptr ClassInfo::superClass() const {
	return m_superClass;
}

ASTree::c_ptr ClassInfo::body() const {
	auto d = std::dynamic_pointer_cast<const ClassStmnt>(definition);
	return d->body();
}

Environment* ClassInfo::environment() const {
	return env;
}

std::string ClassInfo::__str__() const noexcept {
	return "<class " + name() + ">";
}

const std::shared_ptr<const std::string> StoneObject::TYPE{ new std::string("StoneObject") };
StoneObject::StoneObject(Environment* env) : SObject(TYPE), env(env) {}

StoneObject::~StoneObject() {
	delete env;
}

SObject::ptr StoneObject::read(const std::string& member) {
	return getEnv(member)->get(member);
}

void StoneObject::write(const std::string& member, SObject::ptr value) {
	getEnv(member)->putNew(member, value);
}

std::string StoneObject::__str__() const noexcept {
	std::stringstream ss;
	ss << "<object:" << this << ">";
	return ss.str();
}

Environment* StoneObject::getEnv(const std::string& member) {
	auto e = env->where(member);
	if (e && e == env) return e;
	throw StoneException("object member access error: " + member);
}
