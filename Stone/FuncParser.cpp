#include "FuncParser.h"

FuncParser::FuncParser(Lexer& l) : BasicParse(l) {
	param = m_factory.rule<>()->identifier<>(m_reserved);

	params = m_factory.rule<ParameterList>()->ast(param)->repeat(m_factory.rule<>()->sep({ "," })->ast(param));

	paramList = m_factory.rule<>()->sep({ "(" })->maybe(params)->sep({ ")" });

	def = m_factory.rule<DefStmnt>()->sep({ "def" })->identifier<>(m_reserved)->ast(paramList)->ast(block);

	args = m_factory.rule<Arguments>()->ast(expr)->repeat(m_factory.rule<>()->sep({ "," })->ast(expr));

	postfix = m_factory.orRule({ m_factory.rule<>()->sep({ "(" })->maybe(args)->sep({ ")" }) });

	primary->repeat(postfix);

	simple->option(args);

	programOr->push_front({ def });
}
