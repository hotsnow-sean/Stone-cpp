#pragma once

#include "ASTree.h"
#include <stdexcept>

class StoneException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;

	StoneException(const std::string& msg, ASTree::c_ptr tree) : std::runtime_error(msg + " " + tree->location()) {}
};

