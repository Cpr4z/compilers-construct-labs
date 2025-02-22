#pragma once

#include "IAutomationFactory.hpp"

class NFAFactory: public IAutomationFactory {
public:
    IAutomationBuilderPtr CreateStateMachineBuilder() override;
    IAutomationVizuPtr CreateStateMachineVizualizator() override;
};
