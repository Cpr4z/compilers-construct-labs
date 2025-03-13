#pragma once
#include <IAutomationBuilder.hpp>

class NFABuilderImpl;

class NFABuilder: public IAutomationBuilder
{
public:
    //IAutomationBuilder
    NFABuilder();
    ~NFABuilder() override;
    IAutomationPtr Build() override;

    //NFABuilder
    void Init(std::vector<std::string>&& postfix);

private:
    std::unique_ptr<NFABuilderImpl> m_impl;
};

using NFABuilderPtr = std::shared_ptr<NFABuilder>;


