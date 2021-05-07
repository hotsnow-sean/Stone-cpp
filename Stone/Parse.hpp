#pragma once

// 一套使用 ASTree、Lexer 定义的语法分析器生成工具
// 可以把简单的 BNF 文法直接写为对应的 c++ 代码
// 与直接手写逻辑相比，效率会有所下降，但是方便随时修改逻辑组合
// 注意：应使用提供的 ParseFactory 去创建解析规则，这样不用自己管理规则的内存

#include "ASTree.h"
#include "Lexer.h"
#include "ParseException.h"
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <vector>

class Logic {
public:
	virtual ~Logic() = default;
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) = 0;
	virtual bool match(Lexer& l) = 0;
	virtual bool ignore() const noexcept { return false; }
};

class Rule {
public:
	virtual ~Rule() = default;
	virtual ASTree::c_ptr parse(Lexer& l) = 0; // 规则的解析方式
	virtual bool match(Lexer& l) = 0; // 规则是否匹配
};

template <typename T = ASTLeaf>
class NumLogic : public Logic {
public:
	NumLogic() {
		static_assert(std::is_base_of<ASTLeaf, T>::value, "NumLogic template must base of ASTLeaf");
	}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		if (!match(l)) throw ParseException(l.peek(0));
		list.push_back(std::make_shared<T>(l.read()));
	}
	virtual bool match(Lexer& l) override {
		return l.peek(0)->isNumber();
	}
};

template <typename T = ASTLeaf>
class StrLogic : public Logic {
public:
	StrLogic() {
		static_assert(std::is_base_of<ASTLeaf, T>::value, "StrLogic template must base of ASTLeaf");
	}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		if (!match(l)) throw ParseException(l.peek(0));
		list.push_back(std::make_shared<T>(l.read()));
	}
	virtual bool match(Lexer& l) override {
		return l.peek(0)->isString();
	}
};

template <typename T = ASTLeaf>
class IdLogic : public Logic {
private:
	std::unordered_set<std::string> m_reserved;
public:
	IdLogic(const std::unordered_set<std::string>& r) : m_reserved(r) {
		static_assert(std::is_base_of<ASTLeaf, T>::value, "IdLogic template must base of ASTLeaf");
	}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		if (!match(l)) throw ParseException(l.peek(0));
		list.push_back(std::make_shared<T>(l.read()));
	}
	virtual bool match(Lexer& l) override {
		auto t = l.peek(0);
		if (!t->isIdentifier()) return false;
		for (auto r : m_reserved) {
			if (t->getText() == r) return false;
		}
		return true;
	}
};

class SepLogic : public Logic {
protected:
	std::unordered_set<std::string> m_pat;
public:
	SepLogic(const std::unordered_set<std::string>& p) : m_pat(p) {}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		if (!match(l)) throw ParseException(l.peek(0));
		l.read();
	}
	virtual bool match(Lexer& l) override {
		auto t = l.peek(0);
		if (!t->isIdentifier()) return false;
		for (auto r : m_pat) {
			if (t->getText() == r) return true;
		}
		return false;
	}
};

class AstLogic : public Logic {
private:
	Rule* m_rule;
public:
	AstLogic(Rule* r) : m_rule(r) {}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		list.push_back(m_rule->parse(l));
	}
	virtual bool match(Lexer& l) override {
		return m_rule->match(l);
	}
};

class OptionLogic : public Logic {
private:
	Rule* m_rule;
public:
	OptionLogic(Rule* r) : m_rule(r) {}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		if (match(l)) list.push_back(m_rule->parse(l));
	}
	virtual bool match(Lexer& l) override {
		return m_rule->match(l);
	}
	virtual bool ignore() const noexcept override { return true; }
};

template <typename T>
class MaybeLogic : public Logic {
private:
	Rule* m_rule;
public:
	MaybeLogic(Rule* r) : m_rule(r) {
		static_assert(std::is_base_of<ASTList, T>::value, "maybe logic template must base of ASTList");
	}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		if (match(l)) list.push_back(m_rule->parse(l));
		else list.push_back(std::make_shared<T>(std::vector<ASTree::c_ptr>()));
	}
	virtual bool match(Lexer& l) override {
		return m_rule->match(l);
	}
	virtual bool ignore() const noexcept override { return true; }
};

class OrLogic : public Logic {
private:
	std::unordered_set<Rule*> m_rules;
public:
	OrLogic(const std::unordered_set<Rule*>& r) : m_rules(r) {}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		for (auto r : m_rules) {
			if (!r->match(l)) continue;
			list.push_back(r->parse(l));
			return;
		}
	}
	virtual bool match(Lexer& l) override {
		for (auto r : m_rules) {
			if (!r->match(l)) continue;
			return true;
		}
		return false;
	}
};

class RepeatLogic : public Logic {
private:
	Rule* m_rule;
public:
	RepeatLogic(Rule* r) : m_rule(r) {}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		while (match(l)) {
			auto ast = m_rule->parse(l);
			if (ast) list.push_back(ast);
		}
	}
	virtual bool match(Lexer& l) override {
		return m_rule->match(l);
	}
	virtual bool ignore() const noexcept override { return true; }
};

template <typename T>
class ExprLogic : public Logic {
public:
	typedef std::tuple<int, bool> Precedence; // 运算符优先级，运算符是否左结合
private:
	Rule* m_factor;
	std::unordered_map<std::string, Precedence> m_operators;
public:
	ExprLogic(Rule* f, const std::unordered_map<std::string, Precedence>& op) : m_factor(f), m_operators(op) {
		static_assert(std::is_base_of<ASTList, T>::value, "ExprLogic template must base of ASTList");
	}
	// 通过 Logic 继承
	virtual void parse(Lexer& l, std::vector<ASTree::c_ptr>& list) override {
		auto right = m_factor->parse(l);
		Precedence next;
		while (nextOperator(l, next)) {
			right = doShift(l, right, std::get<0>(next));
		}
		list.push_back(right);
	}
	virtual bool match(Lexer& l) override {
		return m_factor->match(l);
	}
private:
	bool rightIsExpr(int prec, Precedence nextPrec) {
		if (std::get<1>(nextPrec)) return prec < std::get<0>(nextPrec);
		return prec <= std::get<0>(nextPrec);
	}
	ASTree::c_ptr doShift(Lexer& l, ASTree::c_ptr left, int prec) {
		ASTree::c_ptr op = std::make_shared<const ASTLeaf>(l.read());
		auto right = m_factor->parse(l);
		Precedence next;
		while (nextOperator(l, next) && rightIsExpr(prec, next)) {
			right = doShift(l, right, std::get<0>(next));
		}
		return std::make_shared<T>(std::vector<ASTree::c_ptr>{left, op, right});
	}
	bool nextOperator(Lexer& l, Precedence& p) {
		auto t = l.peek(0);
		if (t->isIdentifier() && m_operators.find(t->getText()) != m_operators.end()) {
			p = m_operators.at(t->getText());
			return true;
		}
		return false;
	}
};

// 列表规则，即将所给逻辑依次匹配，最后用所有匹配结果构建一个语法树分支点
template <typename T = ASTList>
class ListRule : public Rule {
private:
	std::vector<Logic*> m_rules;
	bool m_only;

public:
	ListRule(bool only = false) : m_only(only) {
		static_assert(std::is_base_of<ASTList, T>::value, "ListRule template must base ASTList");
		if (typeid(T) == typeid(ASTList)) m_only = true;
	}
	~ListRule() {
		for (auto r : m_rules) delete r;
	}

	template <typename E = ASTLeaf>
	ListRule<T>* number() {
		m_rules.push_back(new NumLogic<E>());
		return this;
	}

	template <typename E = ASTLeaf>
	ListRule<T>* string() {
		m_rules.push_back(new StrLogic<E>());
		return this;
	}

	template <typename E = ASTLeaf>
	ListRule<T>* identifier(const std::unordered_set<std::string>& r) {
		m_rules.push_back(new IdLogic<E>(r));
		return this;
	}

	ListRule<T>* sep(const std::unordered_set<std::string>& p) {
		m_rules.push_back(new SepLogic(p));
		return this;
	}

	ListRule<T>* ast(Rule* r) {
		m_rules.push_back(new AstLogic(r));
		return this;
	}

	ListRule<T>* option(Rule* r) {
		m_rules.push_back(new OptionLogic(r));
		return this;
	}

	template <typename E>
	ListRule<T>* maybe(ListRule<E>* r) {
		m_rules.push_back(new MaybeLogic<E>(r));
		return this;
	}

	ListRule<T>* Or(const std::unordered_set<Rule*>& r) {
		m_rules.push_back(new OrLogic(r));
		return this;
	}

	ListRule<T>* repeat(Rule* r) {
		m_rules.push_back(new RepeatLogic(r));
		return this;
	}

	template <typename E>
	ListRule<T>* expression(Rule* f, const std::unordered_map<std::string, std::tuple<int, bool>>& op) {
		m_rules.push_back(new ExprLogic<E>(f, op));
		return this;
	}

	void reset() noexcept {
		for (auto r : m_rules) delete r;
		m_rules.clear();
	}

	void pop() noexcept {
		if (m_rules.size()) {
			delete m_rules.back();
			m_rules.pop_back();
		}
	}

	// 通过 Rule 继承
	virtual ASTree::c_ptr parse(Lexer& l) override {
		std::vector<ASTree::c_ptr> list;
		for (auto r : m_rules) {
			r->parse(l, list);
		}
		if (m_only) {
			if (list.empty()) return nullptr;
			if (list.size() == 1) return list.front();
		}
		return std::make_shared<T>(list);
	}
	virtual bool match(Lexer& l) override {
		for (auto r : m_rules) {
			if (!r->ignore()) return r->match(l);
			if (r->match(l)) return true;
		}
		return true;
	}
};

// 或规则，按照所给规则依次判断，返回第一个匹配上所返回的结点
class OrRule : public Rule {
private:
	std::vector<Rule*> m_rules;

public:
	OrRule(const std::vector<Rule*>& r) : m_rules(r) {}

	void push_back(const std::vector<Rule*>& r) {
		m_rules.insert(m_rules.end(), r.begin(), r.end());
	}
	void push_front(const std::vector<Rule*>& r) {
		m_rules.insert(m_rules.begin(), r.begin(), r.end());
	}

	// 通过 Rule 继承
	virtual ASTree::c_ptr parse(Lexer& l) override {
		for (auto r : m_rules) {
			if (!r->match(l)) continue;
			return r->parse(l);
		}
		return nullptr;
	}
	virtual bool match(Lexer& l) override {
		for (auto r : m_rules) {
			if (r->match(l)) return true;
		}
		return false;
	}
};

// 规则工厂，管理创建规则的内存
class ParseFactory {
private:
	std::vector<Rule*> m_rules;

public:
	~ParseFactory() {
		for (auto r : m_rules) delete r;
	}

	template <typename T = ASTList>
	ListRule<T>* rule(bool only = false) {
		auto ret = new ListRule<T>(only);
		m_rules.push_back(ret);
		return ret;
	}

	OrRule* orRule(const std::vector<Rule*>& r) {
		auto ret = new OrRule(r);
		m_rules.push_back(ret);
		return ret;
	}
};
