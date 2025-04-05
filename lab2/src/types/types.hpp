#pragma once
#include <set>
#include <unordered_map>
#include <vector>

namespace BasicTypes
{
    template<typename T>
    using Vector = std::vector<T>;

    template<typename T>
    using Set = std::set<T>;

    template<typename Key, typename Val>
    using UMap = std::unordered_map<Key, Val>;
}

template<typename T>
struct std::hash<BasicTypes::Vector<T>>
{
size_t operator()(const BasicTypes::Vector<T>& vector) const noexcept
{
    size_t hash = std::hash<size_t>{}(vector.size());
    for (auto&& item : vector)
    {
        hash ^= std::hash<T>{}(item) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}
};

