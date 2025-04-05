#pragma once
#include "types.hpp"

namespace GrammarTypes
{
    using Token = std::string;
    using String = BasicTypes::Vector<Token>;
    using Alphabet = BasicTypes::Set<Token>;
    using ProductionRules = BasicTypes::UMap<String, BasicTypes::Set<String>>;

    const static String kEpsilon {};
}
