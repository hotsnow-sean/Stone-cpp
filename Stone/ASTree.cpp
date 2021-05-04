#include "ASTree.h"
#include "StoneException.h"
#include <stdexcept>

ASTLeaf::ASTLeaf(Token::c_ptr t) : m_token(t) {}

bool ASTLeaf::isLeaf() const noexcept {
    return true;
}

int ASTLeaf::numChildren() const noexcept {
    return 0;
}

Token::c_ptr ASTLeaf::token() const noexcept {
    return m_token;
}

ASTree::c_ptr ASTLeaf::child(size_t i) const {
    throw std::out_of_range("this ast node cannot get child");
}

std::string ASTLeaf::location() const noexcept {
    return "at line " + m_token->getLineNumber();
}

std::string ASTLeaf::toString() const noexcept {
    return m_token->getText();
}

ASTList::ASTList(const std::vector<ASTree::c_ptr>& list) : m_children(list) {}

bool ASTList::isLeaf() const noexcept {
    return false;
}

int ASTList::numChildren() const noexcept {
    return m_children.size();
}

Token::c_ptr ASTList::token() const {
    throw StoneException("this ast node can not get token");
}

ASTree::c_ptr ASTList::child(size_t i) const {
    return m_children.at(i);
}

std::string ASTList::location() const {
    for (auto t : m_children) {
        std::string s = t->location();
        if (s.size()) return s;
    }
    return "";
}

std::string ASTList::toString() const {
    std::string builder("(");
    std::string sep;
    for (auto t : m_children) {
        builder.append(sep);
        sep = " ";
        builder.append(t->toString());
    }
    return builder + ")";
}
