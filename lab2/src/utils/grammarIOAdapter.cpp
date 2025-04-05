#include "grammarIOAdapter.hpp"
#include <fstream>

namespace Utils
{
    Grammar GrammarIOAdapter::readFromFile(const fs_path& path)
    {
        Grammar grammar;
        std::ifstream stream{path};
        if (!stream.is_open())
        {
            throw std::invalid_argument("[Grammar::readFromFile] " + path.string() + ": no such file");
        }
        stream >> grammar;
        return grammar;
    }

    void GrammarIOAdapter::writeToFile(const Grammar& grammar, const fs_path& path)
    {
        std::ofstream stream{path};
        stream << grammar;
    }

}
