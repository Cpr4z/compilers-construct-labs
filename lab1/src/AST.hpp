#pragma once
#include "ASTNode.hpp"
#include <map>

using ASTPtr = std::shared_ptr<class AST>;

class [[nodiscard]] AST
{
public:
    static ASTPtr Instance();
    void Init(const std::vector<std::string>& rpn);
    void Build();
    const ASTNodePtr& GetRoot() const { return m_root; };
    std::map<int, std::string> GetPosToChar() const { return m_posToChar; }
    std::map<int, States> GetFollowPos() const { return m_followpos; }

private:
    ASTNodePtr m_root;
    std::string m_sequence;
    std::map<int, std::string> m_posToChar;
    std::map<int, States> m_followpos;
    int m_posCount = 0;
};