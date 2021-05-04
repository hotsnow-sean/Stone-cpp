#include "Lexer.h"
#include "BasicParse.h"
#include "BasicEnv.h"
#include <iostream>
using namespace std;

int main() {
	Lexer l(cin); // �ִ����������׼��������Ҳ���Դ����ļ�������
	BasicParse bp(l); // �﷨������
	BasicEnv env; // �����Ļ���������
	while (l.peek(0) != Token::eof) {
		auto ast = bp.parse();
		auto r = ast->eval(env);
		cout << "=> " << r->__str__() << endl;
	}
	return 0;
}