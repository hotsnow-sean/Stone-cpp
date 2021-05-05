#pragma once

#include "Environment.h"
#include <unordered_map>

class NestedEnv : public Environment
{
protected:
	std::unordered_map<std::string, SObject::ptr> m_values;
	Environment* outer = nullptr;

public:
	NestedEnv() = default;
	explicit NestedEnv(Environment* e);

	void setOuter(Environment* e) final;
	SObject::ptr get(const std::string& name) final;
	void putNew(const std::string& name, SObject::ptr value) final;
	void put(const std::string& name, SObject::ptr value) final;
	Environment* where(const std::string& name) final;
};

