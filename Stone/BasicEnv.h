#pragma once

#include "Environment.h"
#include <unordered_map>

class BasicEnv : public Environment
{
protected:
	std::unordered_map<std::string, SObject::ptr> m_values;

public:
	// ͨ�� Environment �̳�
	virtual void put(const std::string& name, SObject::ptr value) override;

	virtual SObject::ptr get(const std::string& name) override;
};

