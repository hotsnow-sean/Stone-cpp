#pragma once

#include "StoneException.h"
#include <memory>
#include <string>

class SObject {
public:
	typedef std::shared_ptr<SObject> ptr;
	typedef std::shared_ptr<const SObject> c_ptr;

	const std::shared_ptr<const std::string> __name__; // 类型名

public:
	SObject(const std::shared_ptr<const std::string>& name);

	virtual bool __lt__(SObject::c_ptr other, bool once = false) const;
	virtual bool __gt__(SObject::c_ptr other, bool once = false) const;
	virtual bool __eq__(SObject::c_ptr other, bool once = false) const;
	virtual bool __bool__() const noexcept;
	virtual SObject::ptr __add__(SObject::c_ptr other, bool once = false) const;
	virtual SObject::ptr __sub__(SObject::c_ptr other, bool once = true) const;
	virtual SObject::ptr __mul__(SObject::c_ptr other, bool once = false) const;
	virtual SObject::ptr __div__(SObject::c_ptr other, bool once = true) const;
	virtual SObject::ptr __mod__(SObject::c_ptr other, bool once = true) const;
	virtual std::string __str__() const noexcept;

	static SObject::ptr getNull() noexcept;
	static SObject::ptr createInterger(int v) noexcept;
	static SObject::ptr createString(const std::string& v) noexcept;
	static SObject::ptr createBoolean(bool v) noexcept;
	static int getNumber(SObject::c_ptr o); // 类型错误时抛异常
};