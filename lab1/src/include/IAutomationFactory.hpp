#pragma once

#include "IAutomationBuilder.hpp"
#include "IAutomationVizu.hpp"

class IAutomationFactory
{
public:
    virtual ~IAutomationFactory() = 0;
    virtual IAutomationBuilderPtr CreateStateMachineBuilder() = 0;
    virtual IAutomationVizuPtr CreateStateMachineVizualizator() = 0;
};

using IAutomationFactoryPtr = std::shared_ptr<IAutomationFactory>;
