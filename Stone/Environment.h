#pragma once

#include "SObject.h"

// �����ӿڣ����ڴ�� Stone ����ʱ�����Ķ���

class Environment {
public:
	virtual void put(const std::string& name, SObject::ptr value) = 0;
	virtual SObject::ptr get(const std::string& name) = 0;
};