#pragma once
#include "IAutomationFactory.hpp"


class DFAFactory: public IAutomationFactory
{
public:
    IAutomationBuilderPtr CreateStateMachineBuilder() override;
    IAutomationVizuPtr CreateStateMachineVizualizator() override;
};

