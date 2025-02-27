#include "DFABuilder.hpp"

#include <queue>
//#include <vector>

#include "DFA.hpp"
#include "Utils.h"
//#include "DFAState.hpp"

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
    std::map<StateSet, DFAStatePtr> dfaStates;
    std::queue<StateSet> stateQueue;
    size_t id = 0;
    DFAStatePtr startState = dfa->CreateState(id++);

    startClosure.insert(nfa->GetStart());
    startClosure = utils::Transformation::GetEpsilonClosure(startClosure);

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
//        std::map<std::string, std::vector<NFAStatePtr>> moveTable;
        for (const NFAStatePtr& state: currentState)
        {
            for (const auto& [symbol, nextState] : state->m_transitions)
            {
                if (symbol != utils::tokenConstants::EPSILON)
                {
                      auto converted = utils::Transformation::ToSet(nextState);
                      moveTable[symbol] = converted;
                }
            }
        }

        for (auto& [symbol, nfaNextStates] : moveTable)
        {
//            auto converted = utils::Transformation::ToSet(nfaNextStates);
            StateSet newStateSet = utils::Transformation::GetEpsilonClosure(nfaNextStates);
//            StateSet newStateSet = utils::Transformation::GetEpsilonClosure(nfaNextStates);

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
