#include "ClassParser.h"

static const std::unordered_set<std::string> reserved{ ";",")","}",Token::eol };

ClassParser::ClassParser(Lexer& l) : FuncParser(l) {
	member = m_factory.orRule({ def, simple });

	classBody = m_factory.rule<ClassBody>()->sep({ "{" })->option(member)->repeat(m_factory.rule()->sep({ ";",Token::eol })->option(member))->sep({ "}" });

	defclass = m_factory.rule<ClassStmnt>()->sep({ "class" })->identifier(reserved)->option(m_factory.rule()->sep({ "extends" })->identifier(reserved))->ast(classBody);

	postfix = m_factory.orRule({ m_factory.rule<Dot>()->sep({"."})->identifier(reserved), postfix });

	primary->pop();
	primary->repeat(postfix);

	program->reset();
	program->Or({ defclass, def, statement, m_factory.rule<NullStmnt>() })->sep({ ";", Token::eol });
}
