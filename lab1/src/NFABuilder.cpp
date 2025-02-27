#include "NFABuilder.hpp"

#include <string>
#include <vector>

#include "DFA.hpp"
#include "MyStack.hpp"
#include "NFA.hpp"

class NFABuilderImpl
{
public:
    IAutomationPtr build();
    void init(std::vector<std::string>&& postfixNotation);

private:
    std::vector<std::string> m_postfixNotation;
};

IAutomationPtr NFABuilderImpl::build()
{
    MyStack<NFAPtr> nfaStack;
    StateId id = 0;
    NFAPtr nfa = NFA::Instance();
    for (const auto& token: m_postfixNotation)
    {
        if (token == ".")
        {
            if (nfaStack.size() > 1)
            {
                NFAPtr secondAutomat, firstAutomat;
                if (nfaStack.pop(secondAutomat) && nfaStack.pop(firstAutomat))
                {
                    nfaStack.emplace_back(nfa->CreateConcatAutomat(firstAutomat, secondAutomat));
                }
            }
        }
        else if (token == "|")
        {
            NFAPtr secondAutomat, firstAutomat;
            if (nfaStack.pop(secondAutomat) && nfaStack.pop(firstAutomat))
            {
                nfaStack.emplace_back(nfa->CreateAlternateAutomat(firstAutomat, secondAutomat, id));
            }
        }
        else if (token == "*")
        {
            NFAPtr automat;
            if (nfaStack.pop(automat))
            {
                nfaStack.emplace_back(nfa->CreateKleeneAutomat(automat, id));
            }
        }
        else
        {
            nfaStack.emplace_back(nfa->CreateBaseAutomat(token, id));
        }
    }
    return nfaStack.top();
}

void NFABuilderImpl::init(std::vector<std::string>&& postfixNotation)
{
    m_postfixNotation = std::move(postfixNotation);
}
//////////////////////////////////////////////////////////////////////////////
NFABuilder::NFABuilder(): m_impl(std::make_unique<NFABuilderImpl>())
{
}

NFABuilder::~NFABuilder() = default;

void NFABuilder::Init(std::vector<std::string>&& postfix)
{
    m_impl->init(std::move(postfix));
}

IAutomationPtr NFABuilder::Build()
{
    return m_impl->build();
}
