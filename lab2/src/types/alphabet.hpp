#pragma once
#include "types.hpp"

namespace GrammarTypes
{
    using Token = std::string;
    using String = BasicTypes::Vector<Token>;
    using Alphabet = BasicTypes::Set<Token>;
    using ProductionRules = BasicTypes::UMap<String, BasicTypes::Set<String>>;

    constexpr static std::string_view kEpsilon = "\u03B5";
}
