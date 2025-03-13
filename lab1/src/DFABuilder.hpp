#pragma once
#include "IAutomationBuilder.hpp"
#include "NFA.hpp"

class DFABuilder: public IAutomationBuilder
{
public:
    IAutomationPtr Build() override;
    void Init(const NFAWPtr& nfa);

private:
    NFAWPtr m_nfa;
};

using DFABuilderPtr = std::shared_ptr<DFABuilder>;
