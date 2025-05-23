#include "node.hpp"

void Node::to_postfix(std::ostream& out)
{
    const std::string& val = m_data.m_value;

    if (val == "<программа>")
    {
        out << "{ ";
        for (auto& child : m_children)
            child.to_postfix(out);
        out << "} ";
        return;
    }

    if (val == "<блок>" || val == "<список операторов>")
    {
        for (auto& child : m_children)
            child.to_postfix(out);
        return;
    }

    if (val == "<оператор>" && m_children.size() == 3 && m_children[1].m_data.m_value == "=")
    {
        m_children[0].to_postfix(out);
        m_children[2].to_postfix(out);
        out << "=; ";
        return;
    }

    if (val == "<выражение>")
    {
        m_children[0].to_postfix(out);
        if (m_children.size() == 3)
        {
            m_children[2].to_postfix(out);
            out << m_children[1].m_data.m_value << " ";
        }
        return;
    }

    if (val == "<простое выражение>")
    {
        m_children[0].to_postfix(out);
        if (m_children.size() == 3)
        {
            m_children[2].to_postfix(out);
            out << m_children[1].m_data.m_value << " ";
        }
        return;
    }

    if (val == "<терм>")
    {
        m_children[0].to_postfix(out);
        if (m_children.size() == 3)
        {
            m_children[2].to_postfix(out);
            out << m_children[1].m_data.m_value << " ";
        }
        return;
    }

    if (val == "<фактор>")
    {
        if (m_children.size() == 2 &&
            (m_children[0].m_data.m_tokenType == Grammar::TokenType::KeywordNot ||
             m_children[0].m_data.m_tokenType == Grammar::TokenType::PlusMinus))
        {
            m_children[1].to_postfix(out);
            out << m_children[0].m_data.m_value << " ";
            return;
        }
        else if (m_children.size() == 1)
        {
            m_children[0].to_postfix(out);
            return;
        }
    }

    if (m_data.m_tokenType == Grammar::TokenType::Identifier ||
        m_data.m_tokenType == Grammar::TokenType::KeywordConst)
    {
        out << m_data.m_value << " ";
        return;
    }

    for (auto& ch : m_children)
        ch.to_postfix(out);
}