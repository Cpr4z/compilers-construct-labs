#include "AST.hpp"
#include "MyStack.hpp"
#include <ranges>
#include <vector>

ASTPtr AST::Instance()
{
    return std::make_shared<AST>();
}

void AST::Init(const std::vector<std::string>& rpn)
{
    for (const auto& tkn : rpn)
    {
        m_sequence.insert(m_sequence.size(), tkn);
    }
}

void AST::Build()
{
    MyStack<ASTNodePtr> astStack;
    for (auto&& c : m_sequence | std::views::transform([](char tkn)
    {
        return std::string{tkn};
    }))
    {
        if (std::isalnum(c.front()))
        {
            ASTNodePtr node = std::make_shared<ASTNode>(c, nullptr, nullptr);
            node->AddFirstPos(m_posCount);
            node->AddLastPos(m_posCount);
            m_posToChar[m_posCount++] = c;
            astStack.push(node);
        }
        else
        {
            ASTNodePtr right, left;
            astStack.pop(right);
            if (c != "*")
            {
                astStack.pop(left);
            }
            ASTNodePtr node = std::make_shared<ASTNode>(c, left, right);
            if (c == "|")
            {
                if (left && right)
                {
                    node->SetNullable(left->IsNullable());
                    node->SetFirstPos(left->GetFirstPos());
                    node->SetLastPos(left->GetLastPos());

                    node->SetNullable(node->IsNullable() || right->IsNullable());
                    const States &rightFirstPos = right->GetFirstPos();
                    const States &rightLastPos = right->GetLastPos();
                    States& nodeFirstPos = node->GetFirstPos();
                    States& nodeLastPos = node->GetLastPos();
                    nodeFirstPos.insert(rightFirstPos.cbegin(), rightFirstPos.cend());
                    nodeLastPos.insert(rightLastPos.cbegin(), rightLastPos.cend());
                }
            }
            else if (c == ".")
            {
                if (left && right)
                {
                    node->SetNullable(left->IsNullable() && right->IsNullable());
                    node->SetFirstPos(left->GetFirstPos());
                    if (left->IsNullable())
                    {
                        const States& rightFirstPos = right->GetFirstPos();
                        States& nodeFirstPos = node->GetFirstPos();
                        nodeFirstPos.insert(rightFirstPos.cbegin(), rightFirstPos.cend());
                    }

                    node->SetLastPos(right->GetLastPos());
                    if (right->IsNullable())
                    {
                        const States& leftLastPos = left->GetLastPos();
                        States& nodeLastPos = node->GetLastPos();
                        nodeLastPos.insert(leftLastPos.cbegin(), leftLastPos.cend());
                    }

                    for (size_t p : left->GetLastPos())
                    {
                        const States& rightFirstPos = right->GetFirstPos();
                        m_followpos[p].insert(rightFirstPos.cbegin(), rightFirstPos.cend());
                    }
                }
            }
            else if (c == "*")
            {
                node->SetNullable(true);
                if (right)
                {
                    node->SetFirstPos(right->GetFirstPos());
                    node->SetLastPos(right->GetLastPos());
                    for (size_t p : right->GetLastPos())
                    {
                        States rightFirstPos = right->GetFirstPos();
                        m_followpos[p].insert(rightFirstPos.cbegin(), rightFirstPos.cend());
                    }
                }
            }
            astStack.push(node);
        }
    }
    if (!astStack.empty())
    {
        astStack.pop(m_root);
    }
}
