#include "DFAFactory.hpp"

#include "AutomationVizualizator.hpp"
#include "DFABuilder.hpp"

IAutomationBuilderPtr DFAFactory::CreateStateMachineBuilder()
{
    return std::make_shared<DFABuilder>();
}

IAutomationVizuPtr DFAFactory::CreateStateMachineVizualizator()
{
    return std::make_shared<AutomationVizualizator>();
}
