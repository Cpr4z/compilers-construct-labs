#pragma once
#include <memory>
#include <set>
#include <string>

using ASTNodePtr = std::shared_ptr<class ASTNode>;
using States = std::set<size_t>;

class [[nodiscard]] ASTNode
{
public:
    ASTNode(const std::string& token, const ASTNodePtr& left, const ASTNodePtr& right, int id);

    void AddFirstPos(int num);
    void SetFirstPos(const States& firstPos);
    States& GetFirstPos();

    void AddLastPos(int num);
    States& GetLastPos();
    void SetLastPos(const States& lastPos);

    const std::string& GetToken() const;

    int GetId() const;

    const ASTNodePtr& GetLeft() const;
    const ASTNodePtr& GetRight() const;

    void SetNullable(bool val);
    bool IsNullable() const;

private:
    std::string m_token;
    int m_id = 0;
    ASTNodePtr m_left, m_right;
    bool m_nullable = false; // whether this token can be null symbol
    States m_firstPos, m_lastPos;
};

struct ASTNodeComparator
{
    bool operator()(const ASTNodePtr& lhs, const ASTNodePtr& rhs) const
    {
        return lhs->GetId() < rhs->GetId();
    }
};
