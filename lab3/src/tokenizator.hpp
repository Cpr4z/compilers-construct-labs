#pragma once
#include <vector>
#include "grammar.hpp"

class Tokenizator
{
public:
    [[nodiscard]] std::vector<Grammar::Token> tokenize(std::string&& input);

private:
    void createKeywordToken(const std::string& word);
    void createIdentifierToken(const std::string& word);
    void createBraceParenToken(char tkn);
    void createRelOp1Token(char tkn);
    void createRelOp2Token(const std::string& op);
    void createMulDiv(char tkn);
    void createPlusMinus(char tkn);
    void createAssignToken(char tkn);
    void createSemicolonToken(char tkn);

    static bool isKeyword(const std::string& word) noexcept;
    static bool isBraceParen(char tkn) noexcept;
    static bool isRelOp1(char tkn) noexcept;
    static bool isRelOp2(const std::string& op) noexcept;
    static bool isMulDiv(char tkn) noexcept;
    static bool isPlusMinus(char tkn) noexcept;
    static bool isAssign(char tkn) noexcept;
    static bool isSemicolon(char tkn) noexcept;

private:
    std::vector<Grammar::Token> m_tokens;
};
