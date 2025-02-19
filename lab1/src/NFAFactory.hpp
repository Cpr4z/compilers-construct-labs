#pragma once

#include "IAutomationFactory.hpp"

class NFAFactory: public IAutomationFactory {
public:
    ~NFAFactory() = default;
    IAutomationBuilderPtr CreateStateMachineBuilder() override;
    IAutomationVizuPtr CreateStateMachineVizualizator() override;
};
