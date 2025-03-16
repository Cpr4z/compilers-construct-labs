#pragma once
#include <memory>
#include <set>
#include <string>

using ASTNodePtr = std::shared_ptr<class ASTNode>;
using States = std::set<size_t>;

class [[nodiscard]] ASTNode
{
public:
    ASTNode(std::string token, const ASTNodePtr& left, const ASTNodePtr& right);

    void AddFirstPos(int num);
    void SetFirstPos(const States& firstPos);
    States& GetFirstPos();

    void AddLastPos(int num);
    States& GetLastPos();
    void SetLastPos(const States& lastPos);

    void SetNullable(bool val);
    bool IsNullable() const;

private:
    std::string m_token;
    ASTNodePtr m_left, m_right;
    bool m_nullable = false; // whether this token can be null symbol
    States m_firstPos, m_lastPos;
};
