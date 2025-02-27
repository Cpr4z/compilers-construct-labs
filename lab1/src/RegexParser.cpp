#include "Utils.h"
#include "AutomationFactory.hpp"
#include "NFABuilder.hpp"
#include "DFABuilder.hpp"
#include <string>
#include <iostream>

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

            try
            {
                vizu->CreateVizu(nfa);
            }
            catch (const std::runtime_error& ex)
            {

            }

            factory = AutomationFactory::CreateStateMachineFactory(utils::AutomationType::DFA);
            builder = factory->CreateStateMachineBuilder();

            if (const DFABuilderPtr& dfaBuilder = std::dynamic_pointer_cast<DFABuilder>(builder))
            {
                if (const NFAPtr& nfaPtr = std::dynamic_pointer_cast<NFA>(nfa))
                {
                    dfaBuilder->Init(nfaPtr);
                    IAutomationPtr dfa = dfaBuilder->Build();
                }
            }
        }
    }
    return 0;
}