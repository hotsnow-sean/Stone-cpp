#include "BasicParse.h"

std::unordered_set<std::string> BasicParse::m_reserved{ ")","]","}",";" };

BasicParse::BasicParse(Lexer& l) : m_lexer(l) {
	// ���ﲻ��ֱ���������ʼ��ʱ���룬�������ɾ�̬����������������ʼ��˳��δ֪������
	m_reserved.insert(Token::eol);

	expr = m_factory.rule<>();
	block = m_factory.rule<BlockStmnt>();
	nulltemp = m_factory.rule<NullStmnt>();
	primaryOr = m_factory.orRule({
		m_factory.rule<>()->sep({"("})->ast(expr)->sep({")"}),
		m_factory.rule<>()->number<NumberLiteral>(),
		m_factory.rule<>()->identifier<Name>(m_reserved),
		m_factory.rule<>()->string<StringLiteral>() });

	primary = m_factory.rule<PrimaryExpr>(true)->ast(primaryOr); // �� ast ���� or ֻ��Ϊ�˷����������

	factor = m_factory.orRule({
		m_factory.rule<NegativeExpr>()->sep({"-"})->ast(primary),
		primary });

	expr->expression<BinaryExpr>(factor, {
		{"=", {1, false}},
		{"==", {2, true}},
		{">", {2, true}},
		{"<", {2, true}},
		{"+", {3, true}},
		{"-", {3, true}},
		{"*", {4, true}},
		{"/", {4, true}},
		{"%", {4, true}}
		});

	simple = m_factory.rule<PrimaryExpr>(true)->ast(expr);

	statement = m_factory.orRule({
		m_factory.rule<IfStmnt>()->sep({"if"})->ast(expr)->ast(block)->option(m_factory.rule<>()->sep({"else"})->ast(block)),
		m_factory.rule<WhileStmnt>()->sep({"while"})->ast(expr)->ast(block),
		simple });

	block->sep({ "{" })
		->option(statement)
		->repeat(m_factory.rule<>()->sep({ ";",Token::eol })->option(statement))
		->sep({ "}" });

	programOr = m_factory.orRule({ statement, nulltemp });
	program = m_factory.rule<>()->ast(programOr)->sep({ ";", Token::eol });
}

ASTree::c_ptr BasicParse::parse() {
	return program->parse(m_lexer);
}
