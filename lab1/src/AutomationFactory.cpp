#include "AutomationFactory.hpp"
#include "DFAFactory.hpp"
#include "NFAFactory.hpp"

IAutomationFactoryPtr AutomationFactory::CreateStateMachineFactory(Utils::AutomationType::e type)
{
    switch(type)
    {
        case Utils::AutomationType::NFA: return std::make_shared<NFAFactory>();
        case Utils::AutomationType::DFA: return std::make_shared<DFAFactory>();
        default: return {};
    }
}



