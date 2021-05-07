#include "ClassParser.h"

ClassParser::ClassParser(Lexer& l) : FuncParser(l) {
	member = m_factory.orRule({ def, simple });

	classBody = m_factory.rule<ClassBody>()->sep({ "{" })->option(member)->repeat(m_factory.rule()->sep({ ";",Token::eol })->option(member))->sep({ "}" });

	defclass = m_factory.rule<ClassStmnt>()->sep({ "class" })->identifier(m_reserved)->option(m_factory.rule()->sep({ "extends" })->identifier(m_reserved))->ast(classBody);

	postfix->push_front({ m_factory.rule<Dot>()->sep({"."})->identifier(m_reserved) });

	programOr->push_front({ defclass });
}
