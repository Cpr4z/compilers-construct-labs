#pragma once
#include "IAutomationBuilder.hpp"
#include "AST.hpp"
#include "DFA.hpp"
#include "NFA.hpp"

class DFABuilder: public IAutomationBuilder
{
public:
    IAutomationPtr Build() override;
    void Init(const NFAWPtr& nfa);
    DFAPtr BuildFromAST(const ASTPtr& ast);

private:
    NFAWPtr m_nfa;
};

using DFABuilderPtr = std::shared_ptr<DFABuilder>;
