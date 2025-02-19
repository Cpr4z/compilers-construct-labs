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
    std::getline(std::cin, regex);

    TokensSequence polishedSequence = utils::preprocessing::validateRegex(regex);

    IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(AutomationType::NFA);
    IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();

    if (const auto& nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder); nfaBuilder)
    {
        nfaBuilder->Init(polishedSequence);
        nfaBuilder->Build();
    }


//a*b
//a+
//a*
//a|b
//(a|b)*abb


    return 0;
}