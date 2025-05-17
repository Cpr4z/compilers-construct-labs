#pragma once

#include <string>
#include <vector>


class Parser
{
public:
    static std::pair<bool, std::string> parse(std::vector<std::string> tokens);
};
