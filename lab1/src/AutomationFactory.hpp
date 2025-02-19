#pragma once

#include "IAutomationFactory.hpp"

namespace AutomationType
{
enum e
{
    NFA, FIRST = NFA,
    DFA,
    COUNT
};
}

class AutomationFactory
{
public:
    static IAutomationFactoryPtr CreateStateMachineFactory(AutomationType::e type);
};

