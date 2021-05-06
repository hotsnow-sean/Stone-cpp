#include "BasicParse.h"

BasicParse::BasicParse(Lexer& l) : m_lexer(l) {
	expr = m_factory.rule<>();
	block = m_factory.rule<BlockStmnt>();

	primary = m_factory.rule<PaimaryExpr>(true)->Or({
		m_factory.rule<>()->sep({"("})->ast(expr)->sep({")"}),
		m_factory.rule<>()->number<NumberLiteral>(),
		m_factory.rule<>()->identifier<Name>({ ";","}",Token::eol, ")" }),
		m_factory.rule<>()->string<StringLiteral>() });

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

	simple = m_factory.rule<PaimaryExpr>(true)->ast(expr);

	statement = m_factory.orRule({
		m_factory.rule<IfStmnt>()->sep({"if"})->ast(expr)->ast(block)->option(m_factory.rule<>()->sep({"else"})->ast(block)),
		m_factory.rule<WhileStmnt>()->sep({"while"})->ast(expr)->ast(block),
		simple });

	block->sep({ "{" })
		->option(statement)
		->repeat(m_factory.rule<>()->sep({ ";",Token::eol })->option(statement))
		->sep({ "}" });

	program = m_factory.rule<>()->option(statement)->sep({ ";", Token::eol });
}

ASTree::c_ptr BasicParse::parse() {
	return program->parse(m_lexer);
}
