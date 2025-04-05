#pragma once
#include <ostream>
#include <experimental/iterator>

#include "../types/alphabet.hpp"

namespace Utils
{
namespace ReaderHelpers
{
    constexpr static std::string_view ruleSymbol = "\u2192";
}

namespace IOUtils
{
    template<typename It>
    concept PrintableIterator =
    std::input_iterator<It> &&
    requires (std::ostream& os, It it) {
        { os << *it } -> std::same_as<std::ostream&>;
    };

    template<typename It>
    requires PrintableIterator<It>
    void printContainer(It start, It end, std::ostream& os, std::string del = ", ")
    {
        std::copy(start, end, std::experimental::make_ostream_joiner(os, del));
    }

    template<typename Container>
    concept IterableContainer =
    requires (Container container)
    {
        {std::begin(container)};
        {std::end(container)};
    };

    template<typename Container>
    requires IterableContainer<Container>
    void printContainer(const Container& container, std::ostream& os, std::string del = ", ")
    {
        printContainer(container.cbegin(), container.cend(), os, std::move(del));
    }
} // namespace IOUtils

namespace DecompositionUtils
{
template<typename Container>
auto decomipositeContainer(Container&& container)
{
    if constexpr (std::is_same_v<Container, GrammarTypes::Token>)
    {
        return container | std::views::transform([](const auto& item)
        {
            return GrammarTypes::Token(1, item);
        });
    }
    return container | std::views::all;
}
}

};// namespace Utils
