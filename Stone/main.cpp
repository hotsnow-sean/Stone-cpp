#include "Lexer.h"
#include "BasicParse.h"
#include <iostream>
using namespace std;

int main() {
	Lexer l(cin); // 分词器，传入标准输入流，也可以传入文件输入流
	BasicParse bp(l); // 语法分析器
	while (l.peek(0) != Token::eof) {
		auto ast = bp.parse();
		cout << "=> " << ast->toString() << endl;
	}
	return 0;
}