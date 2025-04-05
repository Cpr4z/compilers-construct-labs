#pragma once
#include <filesystem>
#include "grammar.hpp"

namespace fs = std::filesystem;
using fs_path = fs::path;

namespace Utils
{
class GrammarIOAdapter
{
public:
    Grammar readFromFile(const fs_path& path);
    void writeToFile(const Grammar& grammar, const fs_path& path);
};

}// namespace Utils
