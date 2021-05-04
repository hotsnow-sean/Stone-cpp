#include "Lexer.h"
#include "BasicParse.h"
#include <iostream>
using namespace std;

int main() {
	Lexer l(cin); // �ִ����������׼��������Ҳ���Դ����ļ�������
	BasicParse bp(l); // �﷨������
	while (l.peek(0) != Token::eof) {
		auto ast = bp.parse();
		cout << "=> " << ast->toString() << endl;
	}
	return 0;
}