#include "BasicType.h"
#include "NestedEnv.h"
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
	if (other->__name__ == String::TYPE) return other->__add__(this->shared_from_this());
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