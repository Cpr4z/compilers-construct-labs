#pragma once
#include <variant>
#include <vector>
#include "grammar.hpp"

struct Node
{
    Node() = default;
    explicit Node(Grammar::Token tkn) : m_data(std::move(tkn)) {}

    Grammar::Token m_data;
    std::vector<Node> m_children;

//    Node(const auto& data) : m_data(data) {}
};
