#pragma once

#include "IAutomationFactory.hpp"


class DFAFactory: public IAutomationFactory
{
public:
    ~DFAFactory() = default;
    IAutomationBuilderPtr CreateStateMachineBuilder() override;
    IAutomationVizuPtr CreateStateMachineVizualizator() override;
};

