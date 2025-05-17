#pragma once
#include <filesystem>
#include "node.hpp"

namespace Graphviz
{
void saveDot(const Node& tree, const std::filesystem::path& path);
void GenerateSVGFromDotAndOpen(const std::filesystem::path& path);
}  // namespace Graphviz


