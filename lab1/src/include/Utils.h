#pragma once

#include <set>
#include <type_traits>

#include "NFAState.hpp"

using OperatorPriority = size_t;
using Token = char;
using TokensSequence = std::vector<std::string>;

namespace utils
{
namespace tokenConstants
{
const std::string EPSILON = "eps";

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
[[nodiscard]] TokensSequence validateRegex(std::string&& regex);
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
    DOT, FIRST = DOT,
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

namespace Transformation
{
    using NFAStateSet = std::set<NFAStatePtr, NFAStateComparator>;

    NFAStateSet GetEpsilonClosure(NFAStateSet& states);

    template <typename T, typename = std::void_t<>>
    struct has_key_compare : std::false_type {};

    template <typename T>
    struct has_key_compare<T, std::void_t<typename T::key_compare>> : std::true_type{};

    template <typename ContainerType, bool HasComparator = has_key_compare<ContainerType>::value>
    struct ToSetHelp;

    template <typename ContainerType>
    struct ToSetHelp<ContainerType, false> {
        static std::set<typename ContainerType::value_type> convert(const ContainerType& container)
        {
            return std::set<typename ContainerType::value_type>(container.begin(), container.end());
        }
    };

    template <typename ContainerType>
    struct ToSetHelp<ContainerType, true> {
        static std::set<typename ContainerType::value_type, typename ContainerType::key_compare>
                convert (const ContainerType& container)
        {
            return std::set<typename ContainerType::value_type, typename ContainerType::key_compare>(
                    container.begin(), container.end(), container.key_comp());
        }
    };

    template<typename ContainerType>
    auto ToSet(const ContainerType& container)
    {
        return ToSetHelp<ContainerType>::convert(container);
    }
}
}