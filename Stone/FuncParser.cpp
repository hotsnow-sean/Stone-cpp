#include "FuncParser.h"

FuncParser::FuncParser(Lexer& l) : BasicParse(l) {
	param = m_factory.rule<>()->identifier<>({ ";","}",Token::eol,")" });

	params = m_factory.rule<ParameterList>()->ast(param)->repeat(m_factory.rule<>()->sep({ "," })->ast(param));

	paramList = m_factory.rule<>()->sep({ "(" })->maybe(params)->sep({ ")" });

	def = m_factory.rule<DefStmnt>()->sep({ "def" })->identifier<>({ ";","}",Token::eol,")" })->ast(paramList)->ast(block);

	args = m_factory.rule<Arguments>()->ast(expr)->repeat(m_factory.rule<>()->sep({ "," })->ast(expr));

	postfix = m_factory.rule<>()->sep({ "(" })->maybe(args)->sep({ ")" });

	primary->repeat(postfix);

	simple->option(args);

	program->reset();
	program->Or({ def, statement, m_factory.rule<NullStmnt>() })->sep({ ";", Token::eol });
}
