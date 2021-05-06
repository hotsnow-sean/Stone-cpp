#pragma once

#include <memory>
#include <string>

class SObject {
public:
	typedef std::shared_ptr<SObject> ptr;
	typedef std::shared_ptr<const SObject> c_ptr;

	const std::shared_ptr<const std::string> __name__; // ¿‡–Õ√˚

public:
	SObject(const std::shared_ptr<const std::string>& name);

	virtual bool __lt__(SObject::c_ptr other) const;
	virtual bool __gt__(SObject::c_ptr other) const;
	virtual bool __eq__(SObject::c_ptr other) const;
	virtual bool __bool__() const noexcept;
	virtual SObject::ptr __add__(SObject::c_ptr other) const;
	virtual SObject::ptr __sub__(SObject::c_ptr other) const;
	virtual SObject::ptr __mul__(SObject::c_ptr other) const;
	virtual SObject::ptr __div__(SObject::c_ptr other) const;
	virtual SObject::ptr __mod__(SObject::c_ptr other) const;
	virtual std::string __str__() const noexcept;
};

std::ostream& operator<<(std::ostream& os, const SObject::ptr& obj);