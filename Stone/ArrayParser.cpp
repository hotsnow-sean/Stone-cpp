#include "ArrayParser.h"

ArrayParser::ArrayParser(Lexer& l) : FuncParser(l) {
	elements = m_factory.rule<ArrayLiteral>()->ast(expr)->repeat(m_factory.rule()->sep({ "," })->ast(expr));

	primaryOr->push_front({ m_factory.rule()->sep({"["})->maybe(elements)->sep({"]"}) });

	postfix->push_back({ m_factory.rule<ArrayRef>()->sep({"["})->ast(expr)->sep({"]"}) });
}
