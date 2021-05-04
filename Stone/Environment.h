#pragma once

#include "SObject.h"

// 环境接口，用于存放 Stone 运行时产生的对象

class Environment {
public:
	virtual void put(const std::string& name, SObject::ptr value) = 0;
	virtual SObject::ptr get(const std::string& name) = 0;
};