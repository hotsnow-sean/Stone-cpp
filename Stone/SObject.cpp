#include "SObject.h"
#include "StoneException.h"
#include <ostream>

SObject::SObject(const std::shared_ptr<const std::string>& name) : __name__(name) {}

bool SObject::__lt__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '<'"); }
bool SObject::__gt__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '>'"); }
bool SObject::__eq__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '=='"); }
bool SObject::__bool__() const noexcept { return true; }
SObject::ptr SObject::__add__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '+'"); }
SObject::ptr SObject::__sub__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '-'"); }
SObject::ptr SObject::__mul__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '*'"); }
SObject::ptr SObject::__div__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '/'"); }
SObject::ptr SObject::__mod__(SObject::c_ptr other) const { throw StoneException("'" + *__name__ + "' type cannot '%'"); }
std::string SObject::__str__() const noexcept { return ""; }

std::ostream& operator<<(std::ostream& os, const SObject::ptr& obj) {
	if (obj) os << obj->__str__();
	else os << "null";
	return os;
}
