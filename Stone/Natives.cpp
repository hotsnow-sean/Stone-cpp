#include "Natives.h"
#include "BasicType.h"
#include <iostream>
#include <ctime>

Environment& Natives::environment(Environment& env) {
	appendNatives(env);
	return env;
}

static SObject::ptr print(const std::vector<SObject::ptr>& args) {
	for (auto o : args) std::cout << o << " ";
	std::endl(std::cout);
	return nullptr;
}

static const clock_t startTime = clock();

static SObject::ptr currentTime(const std::vector<SObject::ptr>& args) {
	return SObject::ptr(new Integer((int)(clock() - startTime)));
}

void Natives::appendNatives(Environment& env) {
	env.put("print", SObject::ptr(new NativeFunction("print", &print, -1)));
	env.put("currentTime", SObject::ptr(new NativeFunction("currentTime", &currentTime, 0)));
}
