#include "Lexer.h"
#include "BasicParse.h"
#include "BasicEnv.h"
#include <iostream>
using namespace std;

int main() {
	Lexer l(cin); // 分词器，传入标准输入流，也可以传入文件输入流
	BasicParse bp(l); // 语法分析器
	BasicEnv env; // 上下文环境管理器
	while (l.peek(0) != Token::eof) {
		auto ast = bp.parse();
		auto r = ast->eval(env);
		cout << "=> " << r->__str__() << endl;
	}
	return 0;
}