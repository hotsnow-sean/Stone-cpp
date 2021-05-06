#pragma once

#include "SObject.h"
#include "ASTree.h"

class Integer : public SObject, public std::enable_shared_from_this<Integer>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

private:
	int m_value;

public:
	Integer(int v);

	virtual bool __lt__(SObject::c_ptr other) const override;
	virtual bool __gt__(SObject::c_ptr other) const override;
	virtual bool __eq__(SObject::c_ptr other) const override;
	virtual bool __bool__() const noexcept override;
	virtual SObject::ptr __add__(SObject::c_ptr other) const override;
	virtual SObject::ptr __sub__(SObject::c_ptr other) const override;
	virtual SObject::ptr __mul__(SObject::c_ptr other) const override;
	virtual SObject::ptr __div__(SObject::c_ptr other) const override;
	virtual SObject::ptr __mod__(SObject::c_ptr other) const override;
	virtual std::string __str__() const noexcept override;

	int value() const noexcept;
};

class String : public SObject, public std::enable_shared_from_this<String>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

private:
	std::string m_value;

public:
	String(const std::string& str);

	virtual bool __eq__(SObject::c_ptr other) const override;
	virtual bool __bool__() const noexcept override;
	virtual SObject::ptr __add__(SObject::c_ptr other) const override;
	virtual std::string __str__() const noexcept override;

	std::string value() const noexcept;
};

class Boolean : public SObject, public std::enable_shared_from_this<Boolean>
{
public:
	static const std::shared_ptr<const std::string> TYPE;

private:
	bool m_value;

public:
	Boolean(bool v);

	virtual bool __eq__(SObject::c_ptr other) const override;
	virtual bool __bool__() const noexcept override;
	virtual std::string __str__() const noexcept override;

	bool value() const noexcept;
};

class Function : public SObject
{
public:
	static const std::shared_ptr<const std::string> TYPE;

protected:
	ASTree::c_ptr m_parameters;
	ASTree::c_ptr m_body;
	Environment* env;

public:
	Function(ASTree::c_ptr p, ASTree::c_ptr body, Environment* env);

	ASTree::c_ptr parameters() const;
	ASTree::c_ptr body() const;
	Environment* makeEnv() const;

	std::string __str__() const noexcept final;
};

class NativeFunction : public SObject {
public:
	typedef SObject::ptr(*method_ptr)(const std::vector<SObject::ptr>&);
	static const std::shared_ptr<const std::string> TYPE;

protected:
	std::string name;
	method_ptr method;
	int numParams;

public:
	NativeFunction(const std::string& name, method_ptr m, int argnum);

	int numOfParameters() const noexcept;
	SObject::ptr invoke(const std::vector<SObject::ptr>& args, ASTree::c_ptr);

	std::string __str__() const noexcept final;
};

class ClassInfo : public SObject {
protected:
	ASTree::c_ptr definition;
	SObject::ptr m_superClass;
	Environment* env;

public:
	static const std::shared_ptr<const std::string> TYPE;
	ClassInfo(ASTree::c_ptr cs, Environment* env);

	std::string name() const;
	SObject::ptr superClass() const;
	ASTree::c_ptr body() const;
	Environment* environment() const;

	std::string __str__() const noexcept final;
};

class StoneObject : public SObject {
protected:
	Environment* env; // 保存对象的属性以及方法，在析构函数中释放，所以切勿把自身智能指针保存到env

public:
	static const std::shared_ptr<const std::string> TYPE;
	StoneObject(Environment* env);
	~StoneObject();

	SObject::ptr read(const std::string& member);
	void write(const std::string& member, SObject::ptr value);

	std::string __str__() const noexcept final;

protected:
	Environment* getEnv(const std::string& member);
};
