#include "BasicEnv.h"

void BasicEnv::put(const std::string& name, SObject::ptr value) {
    m_values[name] = value;
}

SObject::ptr BasicEnv::get(const std::string& name) {
    auto ret = m_values.find(name);
    if (ret == m_values.end()) return SObject::getNull();
    return ret->second;
}
