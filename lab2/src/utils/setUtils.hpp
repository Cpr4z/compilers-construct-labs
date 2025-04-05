#pragma once
#include <algorithm>
#include <ranges>
#include <sstream>
#include "../types/alphabet.hpp"
#include "io_utils.hpp"

template<typename T>
std::ostream& operator<<(std::ostream& os, const BasicTypes::Set<T>& set)
{
    std::stringstream ss;
    ss << "{ ";
    Utils::IOUtils::printContainer(set, os);
    ss << " }";
    return os;
}

namespace Utils
{
namespace SetUtils
{
template<typename T>
auto setIntersection(const BasicTypes::Set<T>& first, const BasicTypes::Set<T>& second)
{
    BasicTypes::Set<T> result;
    std::ranges::set_intersection(first, second, std::inserter(result, result.begin()));
    return result;
}

template<typename T>
auto setUnion(const BasicTypes::Set<T>& first, const BasicTypes::Set<T>& second)
{
    BasicTypes::Set<T> result;
    std::ranges::set_union(first, second, std::inserter(result, result.begin()));
    return result;
}

template<typename T>
concept PrintableType =
    requires (std::ostream& os, const T& obj)
    {
    { os << obj } -> std::same_as<std::ostream&>;
    };

template<typename T>
requires PrintableType<T>
std::string toString(const BasicTypes::Set<T>& set)
{
    std::stringstream ss;
    ss << set;
    return ss.str();
}

} //namespace SetUtils

} //namespace Utils
