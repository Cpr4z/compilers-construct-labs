#include "Utils.h"
#include "AutomationFactory.hpp"
#include "NFABuilder.hpp"
#include "DFABuilder.hpp"
#include <string>
#include <iostream>
#include <vector>

#include "DFAMinimizator.hpp"
#include "ObservableProperty.hpp"

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
                        DFAPtr minimizedDFA = minimizator->MinimizeDFA(dfaPtr);

                        result = Utils::Exception::ExecuteNoexcept(
                                [&vizu](const IAutomationPtr &minDFA) { vizu->CreateVizu(minDFA); }, minimizedDFA);

                        // a+b
                        {
                            std::cout << std::boolalpha << minimizedDFA->Imitate("b") << std::endl;
                        }

                        // a+b+c
                        {
                            std::cout << std::boolalpha << minimizedDFA->Imitate("bc") << std::endl;
                            std::cout << std::boolalpha << minimizedDFA->Imitate("ac") << std::endl;
                        }

                        // c*(a|b*)g*d
                        {
                            std::cout << std::boolalpha << minimizedDFA->Imitate("aagd") << std::endl;
                        }

                        // a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))
                        {
                            std::cout << std::boolalpha << minimizedDFA->Imitate("g") << std::endl;
                            std::cout << std::boolalpha << minimizedDFA->Imitate("aggg") << std::endl;
                            std::cout << std::boolalpha << minimizedDFA->Imitate("aaadddd") << std::endl;
                        }

                        // (m|n)*op+(q|r*)s
                        {
                            std::cout << std::boolalpha << minimizedDFA->Imitate("ooprrrs") << std::endl;
                            std::cout << std::boolalpha << minimizedDFA->Imitate("opqrs") << std::endl;
                            std::cout << std::boolalpha << minimizedDFA->Imitate("nnooprs") << std::endl;
                        }

                        // (a|b+).c*(d|ef+).g*
                        {
                            std::cout << std::boolalpha << minimizedDFA->Imitate("d") << std::endl;
                            std::cout << std::boolalpha << minimizedDFA->Imitate("aaefg") << std::endl;
                        }


                    }



                }
            }
        }
    }
    return 0;
}