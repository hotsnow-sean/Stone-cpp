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
	return std::make_shared<Integer>((int)(clock() - startTime));
}

void Natives::appendNatives(Environment& env) {
	env.put("print", std::make_shared<NativeFunction>("print", &print, -1));
	env.put("currentTime", std::make_shared<NativeFunction>("currentTime", &currentTime, 0));
}
