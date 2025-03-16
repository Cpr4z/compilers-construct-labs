#include "ASTNode.hpp"

ASTNode::ASTNode(std::string token, const ASTNodePtr& left, const ASTNodePtr& right) :
    m_token(token), m_left(left), m_right(right)
{
}

void ASTNode::AddFirstPos(int num)
{
    m_firstPos.insert(num);
}

void ASTNode::SetFirstPos(const States& firstPos)
{
    m_firstPos = firstPos;
}

States& ASTNode::GetFirstPos()
{
    return m_firstPos;
}

void ASTNode::AddLastPos(int num)
{
    m_lastPos.insert(num);
}

States& ASTNode::GetLastPos()
{
    return m_lastPos;
}

void ASTNode::SetLastPos(const States& lastPos)
{
    m_lastPos = lastPos;
}

void ASTNode::SetNullable(bool val)
{
    m_nullable = val;
}

bool ASTNode::IsNullable() const
{
    return m_nullable;
}
