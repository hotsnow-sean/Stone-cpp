#include "Lexer.h"
#include "FuncParser.h"
#include "NestedEnv.h"
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
	FuncParser fp(l); // Óï·¨·ÖÎöÆ÷
	NestedEnv env;
	while (l.peek(0) != Token::eof) {
		try {
			auto ast = fp.parse();
			cout << "=> " << ast << endl;
			auto result = ast->eval(env);
			cout << "=> " << result << endl;
		} catch (const std::exception& e) {
			cerr << e.what() << endl;
			return;
		}
	}
}

int main() {
	testFile("D:/02_MyCode/VSrepos/test.txt");
	system("PAUSE");
	return 0;
}