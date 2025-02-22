#pragma once

#include <string>
#include <map>
#include <set>
#include <ranges>
#include <algorithm>
#include <vector>
#include <stack>
#include <iostream>
#include <format>
#include "boost/bimap.hpp"


using OperatorPriority = size_t;
using Token = char;
using TokensSequence = std::vector<std::string>;

namespace utils
{
namespace tokenConstants
{
auto getOtherTokens();
auto getAlphabet();
auto getOperations();
auto mergeAllTokens();
}

namespace preprocessing
{
// a| b valid delete spaces
// a    + valid delete spaces
// a+++ not valid regex
bool isValidRegex(std::string& regex);
void preprocessRegex(std::string& regex);
bool isOperator(Token tkn);
[[nodiscard]] std::vector<std::string> toPostfix(const std::string& regex);
[[nodiscard]] TokensSequence validateRegex(std::string& regex);
}

namespace AutomationType
{
enum e
{
    NFA, FIRST = NFA,
    DFA,
    COUNT
};

namespace file
{
    constexpr std::string_view nfa_file = "nfa_graph";
    constexpr std::string_view dfa_file = "dfa_graph";
}

std::string_view getFileNameByType(e type);
}

namespace VizuType
{
namespace FileType
{
    enum e
    {
        DOT, FIRTS = DOT,
        PNG,
        COUNT,
    };
}

namespace InfoType
{
    enum e
    {
        DIR, FIRST = DIR,
        EXT,
        COUNT,
    };
}

namespace constants
{
    namespace dir
    {
        constexpr std::string_view dot = "Dot";
        constexpr std::string_view png = "Png";
    }

    namespace ext
    {
        constexpr std::string_view dot_ext = ".dot";
        constexpr std::string_view png_ext = ".png";
    }
}

std::string_view getInfo(FileType::e type, InfoType::e infoType);
}
}