#pragma once

#include "IAutomationFactory.hpp"
#include "Utils.h"

class AutomationFactory
{
public:
    static IAutomationFactoryPtr CreateStateMachineFactory(utils::AutomationType::e type);
};

