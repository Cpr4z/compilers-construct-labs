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

    NFAStateSet startClosure;
    NFAStateSet tmpClosure;
    std::map<NFAStateSet, DFAStatePtr> dfaStates;
    std::queue<NFAStateSet> stateQueue;
    size_t id = 0;
    DFAStatePtr startState = dfa->CreateState(id++);

    tmpClosure.insert(nfa->GetStart());
    startClosure = Utils::Transformation::GetEpsilonClosure(tmpClosure);

    std::set<DFAStatePtr> finishStates;


    dfaStates[startClosure] = startState;
    dfa->SetStart(startState);
    stateQueue.push(startClosure);

    while (!stateQueue.empty())
    {
        NFAStateSet currentState = stateQueue.front();
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

        std::map<std::string, NFAStateSet> moveTable;
        for (const NFAStatePtr& state: currentState)
        {
            for (const auto& [symbol, nextState] : state->m_transitions)
            {
                if (symbol != Utils::tokenConstants::EPSILON)
                {
                    moveTable[symbol].insert(nextState.begin(), nextState.end());
                }
            }
        }

        for (auto& [symbol, nfaNextStates] : moveTable)
        {
            NFAStateSet newStateSet = Utils::Transformation::GetEpsilonClosure(nfaNextStates);
            if (!dfaStates.contains(newStateSet))
            {
                DFAStatePtr newState = dfa->CreateState(id++);
                dfaStates[newStateSet] = newState;
                stateQueue.push(newStateSet);
            }
            dfaState->m_transitions.emplace(symbol, dfaStates[newStateSet]);
        }
    }

    for (const auto& [stateSet, dfaState] : dfaStates)
    {
        for (const NFAStatePtr& nfaState : stateSet)
        {
            if (nfaState->m_isFinal)
            {
                finishStates.insert(dfaState);
//                break;
            }
        }
    }

    dfa->SetFinalStates(std::move(finishStates));

    return dfa;
}
