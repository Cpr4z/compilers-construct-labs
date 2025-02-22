#include "Utils.h"
#include "AutomationFactory.hpp"
#include "NFABuilder.hpp"
#include "NFA.hpp"
#include <string>
#include <iostream>


int main()
{
    std::string regex;
    std::string input;

    std::cout << "Please, enter regular expression" << std::endl;
    // a+
    // a*b
    // a*
    // a|b
    // (a|b).a*
    // (a|b)*abb
    while (std::getline(std::cin, regex))
    {
        TokensSequence polishedSequence = utils::preprocessing::validateRegex(regex);
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(utils::AutomationType::e::NFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();

        if (const auto& nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder); nfaBuilder)
        {
            nfaBuilder->Init(std::move(polishedSequence));
            IAutomationPtr nfa = nfaBuilder->Build();
            IAutomationVizuPtr vizu = factory->CreateStateMachineVizualizator();
            try
            {
                vizu->CreateVizu(nfa);
            }
            catch(const std::runtime_error& ex)
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