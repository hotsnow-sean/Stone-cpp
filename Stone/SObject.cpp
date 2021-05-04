#include "SObject.h"

class Null : public SObject, public std::enable_shared_from_this<Null>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

public:
	Null() : SObject(TYPE) {}

	virtual bool __eq__(SObject::c_ptr other, bool once = false) const override { return false; }
	virtual bool __bool__() const noexcept override { return false; }
	virtual std::string __str__() const noexcept override { return "null"; }
};

class Integer : public SObject, public std::enable_shared_from_this<Integer>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

private:
	int m_value;

public:
	Integer(int v) : SObject(TYPE), m_value(v) {}

	virtual bool __lt__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) {
			if (once) throw StoneException("error type for '<': " + *(other->__name__));
			return other->__gt__(this->shared_from_this(), true);
		}
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return m_value < o->value();
	}
	virtual bool __gt__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) {
			if (once) throw StoneException("error type for '>': " + *(other->__name__));
			return other->__lt__(this->shared_from_this(), true);
		}
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return m_value > o->value();
	}
	virtual bool __eq__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) {
			if (once) throw StoneException("error type for '==': " + *(other->__name__));
			return other->__eq__(this->shared_from_this(), true);
		}
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return m_value == o->value();
	}
	virtual bool __bool__() const noexcept override { return m_value != 0; }
	virtual SObject::ptr __add__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) {
			if (once) throw StoneException("error type for '+': " + *(other->__name__));
			return other->__add__(this->shared_from_this(), true);
		}
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return SObject::ptr(new Integer(m_value + o->value()));
	}
	virtual SObject::ptr __sub__(SObject::c_ptr other, bool once = true) const override {
		if (other->__name__ != TYPE) throw StoneException("error type for '-': " + *(other->__name__));
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return SObject::ptr(new Integer(m_value - o->value()));
	}
	virtual SObject::ptr __mul__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) {
			if (once) throw StoneException("error type for '*': " + *(other->__name__));
			return other->__mul__(this->shared_from_this());
		}
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return SObject::ptr(new Integer(m_value * o->value()));
	}
	virtual SObject::ptr __div__(SObject::c_ptr other, bool once = true) const override {
		if (other->__name__ != TYPE) throw StoneException("error type for '/': " + *(other->__name__));
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return SObject::ptr(new Integer(m_value / o->value()));
	}
	virtual SObject::ptr __mod__(SObject::c_ptr other, bool once = true) const override {
		if (other->__name__ != TYPE) throw StoneException("error type for '%': " + *(other->__name__));
		auto o = std::dynamic_pointer_cast<const Integer>(other);
		return SObject::ptr(new Integer(m_value % o->value()));
	}
	virtual std::string __str__() const noexcept override { return std::to_string(m_value); }

	int value() const noexcept { return m_value; }
};

class String : public SObject, public std::enable_shared_from_this<String>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

private:
	std::string m_value;

public:
	String(const std::string& str) : SObject(TYPE), m_value(str) {}

	virtual bool __eq__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) {
			if (once) throw StoneException("error type for '==': " + *(other->__name__));
			return other->__eq__(this->shared_from_this(), true);
		}
		auto o = std::dynamic_pointer_cast<const String>(other);
		return m_value == o->value();
	}
	virtual bool __bool__() const noexcept override { return m_value.size(); }
	virtual SObject::ptr __add__(SObject::c_ptr other, bool once = false) const override {
		return SObject::ptr(new String(m_value + other->__str__()));
	}
	virtual std::string __str__() const noexcept override { return m_value; }

	std::string value() const noexcept { return m_value; }
};

class Boolean : public SObject, public std::enable_shared_from_this<Boolean>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

private:
	bool m_value;

public:
	Boolean(bool v) : SObject(TYPE), m_value(v) {}

	virtual bool __eq__(SObject::c_ptr other, bool once = false) const override {
		if (other->__name__ != TYPE) throw StoneException("error type for '==': " + *(other->__name__));
		auto o = std::dynamic_pointer_cast<const Boolean>(other);
		return m_value == o->value();
	}
	virtual bool __bool__() const noexcept override { return m_value; }
	virtual std::string __str__() const noexcept override { return m_value ? "True" : "False"; }

	bool value() const noexcept { return m_value; }
};

const std::shared_ptr<const std::string> Boolean::TYPE{ new std::string("Boolean") };

const std::shared_ptr<const std::string> String::TYPE{ new std::string("String") };

const std::shared_ptr<const std::string> Integer::TYPE{ new std::string("Integer") };

const std::shared_ptr<const std::string> Null::TYPE{ new std::string("Null") };

SObject::SObject(const std::shared_ptr<const std::string>& name) : __name__(name) {}

bool SObject::__lt__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '<'"); }
bool SObject::__gt__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '>'"); }
bool SObject::__eq__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '=='"); }
bool SObject::__bool__() const noexcept { return true; }
SObject::ptr SObject::__add__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '+'"); }
SObject::ptr SObject::__sub__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '-'"); }
SObject::ptr SObject::__mul__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '*'"); }
SObject::ptr SObject::__div__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '/'"); }
SObject::ptr SObject::__mod__(SObject::c_ptr other, bool once) const { throw StoneException("'" + *__name__ + "' type cannot '%'"); }
std::string SObject::__str__() const noexcept { return ""; }

static SObject::ptr null(new Null());
SObject::ptr SObject::getNull() noexcept {
	return null;
}

SObject::ptr SObject::createInterger(int v) noexcept {
	return SObject::ptr(new Integer(v));
}

SObject::ptr SObject::createString(const std::string& v) noexcept {
	return SObject::ptr(new String(v));
}

SObject::ptr SObject::createBoolean(bool v) noexcept {
	return SObject::ptr(new Boolean(v));
}

int SObject::getNumber(SObject::c_ptr o) {
	if (o->__name__ != Integer::TYPE) throw StoneException("error type for 'getNumber': " + *(o->__name__));
	auto v = std::dynamic_pointer_cast<const Integer>(o);
	return v->value();
}
