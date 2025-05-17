#pragma once
#include <vector>
#include "grammar.hpp"

struct Node
{
    Node() = default;
    explicit Node(Grammar::Token tkn) : m_data(std::move(tkn)) {}
    void to_postfix(std::ostream& out);

    Grammar::Token m_data;
    std::vector<Node> m_children;
};
