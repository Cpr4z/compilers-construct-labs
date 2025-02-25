#include "Utils.h"
#include "AutomationFactory.hpp"
#include "NFABuilder.hpp"
#include <string>
#include <iostream>

int main()
{
    std::string regex;
    std::string input;

    std::cout << "Please, enter regular expression" << std::endl;
    // a+ // not work
    // a*b
    // a*
    // a|b
    // (a|b).a*
    // (a|b)*abb

    while (std::getline(std::cin, regex))
    {
        if (regex.empty())
        {
            return 0;
        }

        TokensSequence polishedSequence = utils::preprocessing::validateRegex(std::move(regex));
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(utils::AutomationType::e::NFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();

        if (const NFABuilderPtr & nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder))
        {
            nfaBuilder->Init(std::move(polishedSequence));
            IAutomationPtr nfa = nfaBuilder->Build();
            IAutomationVizuPtr vizu = factory->CreateStateMachineVizualizator();

            try
            {
                vizu->CreateVizu(nfa);
            }
            catch (const std::runtime_error& ex)
            {

            }
        }
    }


//a*b
//a+
//a*
//a|b
//(a|b)*abb


    return 0;
}