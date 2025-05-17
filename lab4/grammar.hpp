#pragma once
#include <string>
#include <map>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

namespace Grammar
{
enum class TokenType : int
{
    Identifier,
    KeywordConst, // const
    KeywordNot, // not
    KeywordDiv, // div
    KeywordMod, // mod
    KeywordAnd, // and
    KeywordOr, // or
    Assign, // =
    RelOp1, // > <
    RelOp2, // ==, <>, <=, >=
    PlusMinus, // +, -
    MulDiv, // *, /
    OpenParen, // (
    CloseParen, // )
    OpenBrace, // {
    CloseBrace, // }
    Semicolon, // ;
    Operator,
    EndOfInput
};

struct Token
{
    Token() = default;
    explicit Token(TokenType type, std::string value) : m_tokenType(type), m_value(std::move(value)) {}
    TokenType m_tokenType;
    std::string m_value;
};

using TokenMap = boost::bimap<
        boost::bimaps::set_of<std::string_view>,
        boost::bimaps::multiset_of<TokenType>>;
using namespace std::string_view_literals;

inline TokenMap getKeywords()
{
    // "const"
    // "not"
    // "div"
    // "mod"
    // "and"
    // "or"
    static TokenMap keyWords;
    keyWords.insert({"const"sv, TokenType::KeywordConst});
    keyWords.insert({"not"sv, TokenType::KeywordNot});
    keyWords.insert({"div"sv, TokenType::KeywordDiv});
    keyWords.insert({"mod"sv, TokenType::KeywordMod});
    keyWords.insert({"and"sv, TokenType::KeywordAnd});
    keyWords.insert({"or"sv, TokenType::KeywordOr});
    return keyWords;
}

inline TokenMap getBraceParen()
{
    static TokenMap braceParen;
    braceParen.insert({"{"sv, TokenType::OpenBrace});
    braceParen.insert({"}"sv, TokenType::CloseBrace});
    braceParen.insert({"("sv, TokenType::OpenParen});
    braceParen.insert({")"sv, TokenType::CloseParen});
    return braceParen;
}

inline TokenMap getRelOp1()
{
    static TokenMap relOp1;
    relOp1.insert({"<"sv, TokenType::RelOp1});
    relOp1.insert({">"sv, TokenType::RelOp1});
    return relOp1;
}

inline TokenMap getRelOp2()
{
    static TokenMap relOp2;
    // ==, <>, <=, >=
    relOp2.insert({"=="sv, TokenType::RelOp2});
    relOp2.insert({"<>"sv, TokenType::RelOp2});
    relOp2.insert({"<="sv, TokenType::RelOp2});
    relOp2.insert({">="sv, TokenType::RelOp2});
    return relOp2;
}

inline TokenMap getPlusMinus()
{
    static TokenMap plusMinus;
    plusMinus.insert({"-"sv, TokenType::PlusMinus});
    plusMinus.insert({"+"sv, TokenType::PlusMinus});
    return plusMinus;
}

inline TokenMap getMulDiv()
{
    static TokenMap mulDiv;
    mulDiv.insert({"*"sv, TokenType::MulDiv});
    mulDiv.insert({"/"sv, TokenType::MulDiv});
    return mulDiv;
}

} // namespace Grammar
