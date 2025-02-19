#include "NFABuilder.hpp"

#include <string>
#include <vector>

#include "DFA.hpp"
#include "MyStack.hpp"
#include "NFA.hpp"

constexpr Token EPSILON = U'ε';

class NFABuilderImpl
{
public:
    NFABuilderImpl() = default;
    ~NFABuilderImpl() = default;

    IAutomationPtr build();
    void init(const std::vector<std::string>& postfixNotation);

private:
    std::vector<std::string> m_postfixNotation;
};

IAutomationPtr NFABuilderImpl::build()
{
    MyStack<std::pair<StatePtr, StatePtr>> nfaStack;
    StateId id = 0;
    NFAPtr nfa = NFA::Instance();
    for (const auto& token: m_postfixNotation)
    {
        if (token == ".")
        {
            auto nfa2 = nfaStack.top();
            nfaStack.pop();
            auto nfa1 = nfaStack.top();
            nfaStack.pop();

            nfa->AddTransition(nfa1.second, nfa2.first, EPSILON);

            nfaStack.emplace_back(nfa1.first, nfa2.second);
        }
        else if (token == "|")
        {
            auto nfa2 = nfaStack.top();
            nfaStack.pop();
            auto nfa1 = nfaStack.top();
            nfaStack.pop();

            StatePtr start = nfa->CreateState(++id);
            StatePtr accept = nfa->CreateState(++id, true);

            nfa->AddTransition(start, nfa1.first, EPSILON);
            nfa->AddTransition(start, nfa2.first, EPSILON);
            nfa->AddTransition(nfa1.second, accept, EPSILON);
            nfa->AddTransition(nfa2.second, accept, EPSILON);

            nfaStack.emplace_back(start, accept);
        }
        else if (token == "*")
        {
            auto _nfa = nfaStack.top();
            nfaStack.pop();

            StatePtr start = nfa->CreateState(++id);
            StatePtr accept = nfa->CreateState(++id, true);

            nfa->AddTransition(start, _nfa.first, EPSILON);
            nfa->AddTransition(start, accept, EPSILON);
            nfa->AddTransition(_nfa.second, _nfa.first, EPSILON);
            nfa->AddTransition(_nfa.second, accept, EPSILON);

            nfaStack.emplace_back(start, accept);
        }
        else
        {
            StatePtr start = nfa->CreateState(++id);
            StatePtr accept = nfa->CreateState(++id, true);

            // To do: refactor
            nfa->AddTransition(start, accept, static_cast<Token>(token[0]));
            nfaStack.emplace_back(start, accept);
        }
    }

    auto finalStates = nfaStack.top();
    nfaStack.pop();
    nfa->Init(finalStates.first, finalStates.second);

    return nfa;
}

void NFABuilderImpl::init(const std::vector<std::string>& postfixNotation)
{
    m_postfixNotation = postfixNotation;
}
//////////////////////////////////////////////////////////////////////////////
NFABuilder::NFABuilder(): m_impl(std::make_unique<NFABuilderImpl>())
{
}

NFABuilder::~NFABuilder()
{
}


void NFABuilder::Init(const std::vector<std::string>& postfix)
{
    m_impl->init(postfix);
}

IAutomationPtr NFABuilder::Build()
{
    return m_impl->build();
}
