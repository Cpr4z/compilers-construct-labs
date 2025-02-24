#include "Utils.h"
#include "AutomationFactory.hpp"
#include "NFABuilder.hpp"
#include <string>
#include <iostream>
#include <ranges>
#include <algorithm>




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
        std::ranges::for_each(polishedSequence, [](const auto& token){std::cout << token;});
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(utils::AutomationType::e::NFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();

        if (const NFABuilderPtr & nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder))
        {
            nfaBuilder->Init(std::move(polishedSequence));
            IAutomationPtr nfa = nfaBuilder->Build();
            IAutomationVizuPtr vizu = factory->CreateStateMachineVizualizator();

            //a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))
            {
                std::cout << nfa->Imitate("aaabh") << std::endl;
            }

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