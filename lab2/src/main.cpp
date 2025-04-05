#include <filesystem>
#include <functional>
#include <iostream>
#include "../utils/grammarIOAdapter.hpp"
#include "../grammar/contextFreeGrammar.hpp"


using Callback = std::function<void(ContextFreeGrammar&)>;
using CallbackRef = Callback&;

namespace fs = std::filesystem;
using fs_path = fs::path;


bool sTest(const fs_path& grammar_path,
           const fs_path& result_path,
           const fs_path& expected_path,
           CallbackRef callback)
{
    Utils::GrammarIOAdapter ioAdapter;
    ContextFreeGrammar grammar{ioAdapter.readFromFile(grammar_path)};
    const ContextFreeGrammar expected{ioAdapter.readFromFile(expected_path)};
    callback(grammar);
    ioAdapter.writeToFile(grammar, result_path);
    return grammar == expected;
}

bool sEliminateLeftRecursionTest(size_t i)
{
    const auto filepath = [i](const std::string& name) {
        return "../tests/ELR-" + std::to_string(i) + "-" + name + ".txt";
    };

    Callback job = [](ContextFreeGrammar& grammar)
    {
        grammar.eliminateLeftRecursion();
    };
    return sTest(filepath("grammar"), filepath("eliminated"), filepath("expected"), job);
}

bool sRemoveUnreachableSymbolsTest(size_t i)
{
    const auto filepath = [i](const std::string& name) {
        return "../tests/RUS-" + std::to_string(i) + "-" + name + ".txt";
    };

    Callback job = [](ContextFreeGrammar& grammar)
    {
        grammar.removeUnreachableSymbols();
    };
    return sTest(filepath("grammar"), filepath("removedUnreachable"), filepath("expected"), job);
}

int main()
{
    std::cout << "Tests for left recursion: " << std::endl;
    for (size_t i = 1; i <= 6; ++i)
    {
        std::cout << std::boolalpha << sEliminateLeftRecursionTest(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Tests for removing unreachable symbols: " << std::endl;
    for (size_t i = 1; i <= 5; ++i)
    {
        std::cout << std::boolalpha << sRemoveUnreachableSymbolsTest(i) << std::endl;
    }
    return 0;
}
