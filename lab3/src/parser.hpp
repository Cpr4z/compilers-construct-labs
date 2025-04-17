#pragma once
#include <vector>
#include "grammar.hpp"
#include "node.hpp"

class Parser
{
public:
    explicit Parser(std::vector<Grammar::Token> tokens) : m_tokens(std::move(tokens)), m_pos(0) {}

    Node parse();

private:
    Node parseProgram();
    Node parseBlock();
    Node parseOperatorList();
    Node parseOperator();
    Node parseTail();
    Node parseExpression();
    Node parseSimpleExpression();
    Node parseTerm();
    Node parseFactor();

    const Grammar::Token& current() const;
    const Grammar::Token& advance();
    bool match(Grammar::TokenType type);
    void expect(Grammar::TokenType type);
    void rewind(size_t pos) { m_pos = pos; };
    size_t position() const { return m_pos; }

private:
    const std::vector<Grammar::Token> m_tokens;
    size_t m_pos;
};
