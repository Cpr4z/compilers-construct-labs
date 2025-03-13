#include <string>
#include <iostream>
#include <vector>
#include "Utils.h"
#include "AutomationFactory.hpp"
#include "NFABuilder.hpp"
#include "DFABuilder.hpp"
#include "DFAMinimizator.hpp"
#include "ObservableProperty.hpp"

int main()
{
    std::string regex;
    std::string input;
    std::cout << "Please, enter regular expression" << std::endl;
    while (std::getline(std::cin, regex))
    {
        if (regex.empty())
        {
            return 0;
        }

        TokensSequence polishedSequence = Utils::preprocessing::validateRegex(std::move(regex));
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(Utils::AutomationType::e::NFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();
        ObservableProperty<bool> result(true);
        result.Connect([](){
            std::cout << "Something went wrong";
            std::exit(-1);
        });

        if (const NFABuilderPtr & nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder))
        {
            nfaBuilder->Init(std::move(polishedSequence));
            IAutomationPtr nfa = nfaBuilder->Build();
            IAutomationVizuPtr vizu = factory->CreateStateMachineVizualizator();

            result = Utils::Exception::ExecuteNoexcept([&vizu](const IAutomationPtr& nfa){vizu->CreateVizu(nfa);}, nfa);

            IAutomationFactoryPtr factory_ = AutomationFactory::CreateStateMachineFactory(Utils::AutomationType::DFA);
            IAutomationBuilderPtr builder_ = factory_->CreateStateMachineBuilder();

            if (const DFABuilderPtr& dfaBuilder = std::dynamic_pointer_cast<DFABuilder>(builder_))
            {
                if (const NFAPtr& nfaPtr = std::dynamic_pointer_cast<NFA>(nfa))
                {
                    dfaBuilder->Init(nfaPtr);
                    IAutomationPtr dfa = dfaBuilder->Build();
                    result = Utils::Exception::ExecuteNoexcept([&vizu](const IAutomationPtr& dfa){vizu->CreateVizu(dfa);}, dfa);

                    if (const DFAPtr& dfaPtr = std::dynamic_pointer_cast<DFA>(dfa)) {
                        DFAMinimizatorPtr minimizator = DFAMinimizator::Instance();
                        DFAPtr minimizedDFA = minimizator->MinimizeKhophort(dfaPtr);

                        result = Utils::Exception::ExecuteNoexcept(
                                [&vizu](const IAutomationPtr &minDFA) { vizu->CreateVizu(minDFA); }, minimizedDFA);
                    }
                }
            }
        }
    }
    return 0;
}