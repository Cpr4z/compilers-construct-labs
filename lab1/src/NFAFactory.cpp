#include "NFAFactory.hpp"
#include "NFABuilder.hpp"
#include "AutomationVizualizator.hpp"

IAutomationBuilderPtr NFAFactory::CreateStateMachineBuilder()
{
    return std::make_shared<NFABuilder>();
}

IAutomationVizuPtr NFAFactory::CreateStateMachineVizualizator()
{
    return std::make_shared<AutomationVizualizator>();
}
