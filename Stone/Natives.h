#pragma once

#include "Environment.h"

class Natives
{
public:
	static Environment& environment(Environment& env);

protected:
	static void appendNatives(Environment& env);
};

