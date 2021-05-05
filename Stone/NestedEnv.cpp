#include "NestedEnv.h"

NestedEnv::NestedEnv(Environment* e) : outer(e) {}

void NestedEnv::setOuter(Environment* e) {
	outer = e;
}

SObject::ptr NestedEnv::get(const std::string& name) {
	auto ret = m_values.find(name);
	if (ret != m_values.end()) return ret->second;
	if (outer == nullptr) return nullptr;
	return outer->get(name);
}

void NestedEnv::putNew(const std::string& name, SObject::ptr value) {
	m_values[name] = value;
}

void NestedEnv::put(const std::string& name, SObject::ptr value) {
	auto e = where(name);
	if (e == nullptr) e = this;
	e->putNew(name, value);
}

Environment* NestedEnv::where(const std::string& name) {
	if (m_values.find(name) != m_values.end()) return this;
	if (outer == nullptr) return nullptr;
	return outer->where(name);
}
