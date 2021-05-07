#include "Lexer.h"
#include "ParseException.h"

static std::string regexPat{ R"(\s*((//.*)|([0-9]+)|("(?:\\"|\\\\|\\n|[^"])*")|[A-Z_a-z][A-Z_a-z0-9]*|==|<=|>=|&&|\|\||[[:punct:]])?)" };
static std::regex pattern{ regexPat };

// 将被引号包裹的字符串转为存储用的字符串，包括处理 \\ \n \" 的转义
static std::string toStringLiteral(const std::string& s) noexcept {
	std::string sb;
	int len = s.length() - 1;
	for (size_t i = 1; i < len; i++) {
		char c = s[i];
		if (c == '\\' && i + 1 < len) {
			char c2 = s[i + 1];
			if (c2 == '"' || c2 == '\\') {
				c = s[++i];
			} else if (c2 == 'n') {
				i++;
				c = '\n';
			}
		}
		sb += c;
	}
	return sb;
}

class NumToken : public Token {
private:
	int value;

public:
	NumToken(int line, int v) : Token(line), value(v) {}

public:
	bool isNumber() const noexcept override { return true; }
	std::string getText() const noexcept override { return std::to_string(value); }
	int getNumber() const noexcept override { return value; }
};

class IdToken : public Token {
private:
	std::string text;

public:
	IdToken(int line, const std::string& id) : Token(line), text(id) {}

public:
	bool isIdentifier() const noexcept override { return true; }
	std::string getText() const noexcept override { return text; }
};

class StrToken : public Token {
private:
	std::string literal;

public:
	StrToken(int line, const std::string& str) : Token(line), literal(str) {}

public:
	bool isString() const noexcept override { return true; }
	std::string getText() const noexcept override { return literal; }
};

Lexer::Lexer(std::istream& reader) : reader(reader) {}

Token::c_ptr Lexer::read() {
	Token::c_ptr ret = Token::eof;
	if (fillQueue(0)) {
		ret = queue.front();
		queue.pop_front();
	}
	return ret;
}

Token::c_ptr Lexer::peek(size_t i) {
	if (fillQueue(i)) return queue.front();
	return Token::eof;
}

bool Lexer::fillQueue(size_t i) {
	while (i >= queue.size()) {
		if (!has_more) return false;
		readLine();
	}
	return true;
}

void Lexer::readLine() {
	std::string line;
	try {
		while (line.empty() && reader.good()) {
			std::getline(reader, line);
			line_number++;
		}
	} catch (std::ios_base::failure e) {
		throw ParseException(e.what());
	}
	if (reader.eof()) {
		has_more = false;
	} else if (!reader.good()) {
		has_more = false;
		throw ParseException("input stream error: at line " + std::to_string(line_number));
	}
	if (line.empty()) return;
	std::smatch matcher;
	while (line.size() && std::regex_search(line, matcher, pattern)) {
		addToken(line_number, matcher);
		line = matcher.suffix().str();
	}
	if (line.size()) {
		throw ParseException("bad token at line " + std::to_string(line_number));
	}
	queue.push_back(std::make_shared<IdToken>(line_number, Token::eol));
}

void Lexer::addToken(int lineNo, const std::smatch& matcher) {
	std::string m = matcher[1]; // 1 号匹配即去除空格之外的完整匹配字符串
	if (m.empty() || matcher[2].matched) return; // 2 号即注释匹配部分
	Token::c_ptr t;
	if (matcher[3].matched) {
		t = std::make_shared<const NumToken>(lineNo, std::stoi(m));
	} else if (matcher[4].matched) {
		t = std::make_shared<const StrToken>(lineNo, toStringLiteral(m));
	} else {
		t = std::make_shared<const IdToken>(lineNo, m);
	}
	queue.push_back(t);
}
