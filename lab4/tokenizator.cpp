#include "tokenizator.hpp"
#include <ranges>

bool Tokenizator::isKeyword(const std::string& word) noexcept
{
    const auto& keywords = Grammar::getKeywords();
    return keywords.left.find(word) != keywords.left.end();
}

bool Tokenizator::isBraceParen(char tkn) noexcept
{
    const auto& braceParen = Grammar::getBraceParen();
    return braceParen.left.find(std::string(1, tkn)) != braceParen.left.end(); // ?
}

bool Tokenizator::isRelOp1(char tkn) noexcept
{
    const auto& relOp1 = Grammar::getRelOp1();
    return relOp1.left.find(std::string(1, tkn)) != relOp1.left.end();
}

bool Tokenizator::isRelOp2(const std::string& op) noexcept
{
    const auto& relOp2 = Grammar::getRelOp2();
    return relOp2.left.find(std::string_view(op)) != relOp2.left.end();
}

bool Tokenizator::isMulDiv(char tkn) noexcept
{
    const auto& mulDiv = Grammar::getMulDiv();
    return mulDiv.left.find(std::string(1, tkn)) != mulDiv.left.end();
}

bool Tokenizator::isPlusMinus(char tkn) noexcept
{
    const auto& plusMinus = Grammar::getPlusMinus();
    return plusMinus.left.find(std::string(1, tkn)) != plusMinus.left.end();
}

bool Tokenizator::isAssign(char tkn) noexcept
{
    return tkn == '=';
}

bool Tokenizator::isSemicolon(char tkn) noexcept
{
    return tkn == ';';
}

void Tokenizator::createKeywordToken(const std::string& word)
{
    const auto& keywords = Grammar::getKeywords();
    auto it = keywords.left.find(word);
    m_tokens.emplace_back(it->second, word);
}

void Tokenizator::createIdentifierToken(const std::string& word)
{
    m_tokens.emplace_back(Grammar::TokenType::Identifier, word);
}

void Tokenizator::createBraceParenToken(char tkn)
{
    const auto& braceParen = Grammar::getBraceParen();
    auto it = braceParen.left.find(std::string(1, tkn)); // ?
    m_tokens.emplace_back(it->second, std::string(1, tkn)); // ?
}

void Tokenizator::createRelOp1Token(char tkn)
{
    const auto& relOp1 = Grammar::getRelOp1();
    auto it = relOp1.left.find(std::string(1, tkn));
    m_tokens.emplace_back(it->second, std::string(1, tkn));
}

void Tokenizator::createRelOp2Token(const std::string& op)
{
    const auto& relOp2 = Grammar::getRelOp2();
    auto it = relOp2.left.find(op);
    m_tokens.emplace_back(it->second, op);
}

void Tokenizator::createMulDiv(char tkn)
{
    const auto& mulDiv = Grammar::getMulDiv();
    auto it = mulDiv.left.find(std::string(1, tkn));
    m_tokens.emplace_back(it->second, std::string(1, tkn));
}

void Tokenizator::createPlusMinus(char tkn)
{
    const auto& plusMinus = Grammar::getPlusMinus();
    auto it = plusMinus.left.find(std::string(1, tkn));
    m_tokens.emplace_back(it->second, std::string(1, tkn));
}

void Tokenizator::createAssignToken(char tkn)
{
    m_tokens.emplace_back(Grammar::TokenType::Assign, std::string(1, tkn));
}

void Tokenizator::createSemicolonToken(char tkn)
{
    m_tokens.emplace_back(Grammar::TokenType::Semicolon, std::string(1, tkn));
}

std::vector<Grammar::Token> Tokenizator::tokenize(std::string&& input)
{
    using namespace Grammar;
    size_t i = 0;
    auto isLetter = [](char tkn){ return std::isalpha(static_cast<unsigned char>(tkn)); };
    auto isSpace = [](char tkn){ return std::isspace(static_cast<unsigned char>(tkn)); };
    while(i < input.size())
    {
        char currentTkn = input[i];
        if (isSpace(input[i]))
        {
            ++i;
            continue;
        }

        if (isBraceParen(input[i]))
        {
            createBraceParenToken(input[i]);
            ++i;
            continue;
        }
        else if (auto substr = input.substr(i, 2); isRelOp2(substr))
        {
            createRelOp2Token(substr);
            i+= 2;
            continue;
        }
        else if (isAssign(input[i]))
        {
            createAssignToken(input[i]);
            ++i;
            continue;
        }
        else if (isSemicolon(input[i]))
        {
            createSemicolonToken(input[i]);
            ++i;
            continue;
        }
        else if (isRelOp1(input[i]))
        {
            createRelOp1Token(input[i]);
            ++i;
            continue;
        }
        else if (isMulDiv(input[i]))
        {
            createMulDiv(input[i]);
            ++i;
            continue;
        }
        else if (isPlusMinus(input[i]))
        {
            createPlusMinus(input[i]);
            ++i;
            continue;
        }

        if (isLetter(input[i]))
        {
            size_t start = i;
            while (i < input.size() && isLetter(input[i]))
            {
                ++i;
            }
            std::string word = input.substr(start, i - start);
            if (isKeyword(word))
            {
                createKeywordToken(word);
            }
            else
            {
                createIdentifierToken(word);
            }
            continue;
        }
        throw std::runtime_error("Unknown character: " + std::string(1, input[i]));
    }
    return m_tokens;
}
