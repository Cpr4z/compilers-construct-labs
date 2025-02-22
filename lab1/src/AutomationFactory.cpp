#include "AutomationFactory.hpp"

#include "DFAFactory.hpp"
#include "NFAFactory.hpp"

IAutomationFactoryPtr AutomationFactory::CreateStateMachineFactory(utils::AutomationType::e type)
{
    switch(type)
    {
        case utils::AutomationType::NFA: return std::make_shared<NFAFactory>();
        case utils::AutomationType::DFA: return std::make_shared<DFAFactory>();
        default: return {};
    }
}



