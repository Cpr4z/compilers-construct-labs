#include "AutomationFactory.hpp"

#include "DFAFactory.hpp"
#include "NFAFactory.hpp"

IAutomationFactoryPtr AutomationFactory::CreateStateMachineFactory(AutomationType::e type)
{
    switch(type)
    {
        case AutomationType::NFA: return std::make_shared<NFAFactory>();
        case AutomationType::DFA: return std::make_shared<DFAFactory>();
        default: return {};
    }
}



