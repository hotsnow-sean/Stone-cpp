#include "Lexer.h"
#include "ClassParser.h"
#include "NestedEnv.h"
#include "Natives.h"
#include <iostream>
#include <fstream>
using namespace std;

static void testFile(const char* file) {
	ifstream ifs(file);
	if (!ifs.is_open()) {
		cerr << "File open fail... : " << file << endl;
		return;
	}
	Lexer l(ifs);
	ClassParser cp(l); // 类语法分析器
	NestedEnv env;
	Natives::environment(env); // 添加内置函数
	while (l.peek(0) != Token::eof) {
		try {
			/*auto ast = cp.parse();
			cout << "=> " << ast << endl;
			auto o = ast->eval(env);
			cout << "===> " << o << endl;*/
			cp.parse()->eval(env);
		} catch (const std::exception& e) {
			cerr << e.what() << endl;
			ifs.close();
			return;
		}
	}
	ifs.close();
}

int main() {
	testFile("D:/02_MyCode/VSrepos/test.txt");
	system("PAUSE");
	return 0;
}