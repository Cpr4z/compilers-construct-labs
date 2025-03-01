#include "DFABuilder.hpp"

#include <queue>
#include "DFA.hpp"
#include "Utils.h"

void DFABuilder::Init(const NFAWPtr& nfa)
{
    m_nfa = nfa;
}

IAutomationPtr DFABuilder::Build()
{
    if (m_nfa.expired())
    {
        return {};
    }

    const NFAPtr& nfa = m_nfa.lock();
    DFAPtr dfa = DFA::Instance();

    StateSet startClosure;
    StateSet tmpClosure;
    std::map<StateSet, DFAStatePtr> dfaStates;
    std::queue<StateSet> stateQueue;
    size_t id = 0;
    DFAStatePtr startState = dfa->CreateState(id++);

    tmpClosure.insert(nfa->GetStart());
    startClosure = utils::Transformation::GetEpsilonClosure(tmpClosure);

    dfaStates[startClosure] = startState;
    dfa->SetStart(startState);
    stateQueue.push(startClosure);

    while (!stateQueue.empty())
    {
        StateSet currentState = stateQueue.front();
        stateQueue.pop();
        DFAStatePtr dfaState = dfaStates[currentState];

        for (const NFAStatePtr& nfaState: currentState)
        {
            if (nfaState->m_isFinal)
            {
                dfaState->m_isFinal = true;
                break;
            }
        }

        std::map<std::string, StateSet> moveTable;
        for (const NFAStatePtr& state: currentState)
        {
            for (const auto& [symbol, nextState] : state->m_transitions)
            {
                if (symbol != utils::tokenConstants::EPSILON)
                {
                      moveTable[symbol] = nextState;
                }
            }
        }

        for (auto& [symbol, nfaNextStates] : moveTable)
        {
            StateSet newStateSet = utils::Transformation::GetEpsilonClosure(nfaNextStates);
            auto it = dfaStates.find(newStateSet);
            if (it == dfaStates.end())
            {
                DFAStatePtr newState = dfa->CreateState(id++);
                dfaStates[newStateSet] = newState;
                stateQueue.push(newStateSet);
            }
            dfaState->m_transitions[symbol] = dfaStates[newStateSet];
        }

    }
    return dfa;
}
