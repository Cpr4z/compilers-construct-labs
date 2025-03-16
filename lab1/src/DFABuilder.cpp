#include "DFABuilder.hpp"
#include <ranges>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "DFA.hpp"
#include "Utils.h"

struct StatesHash
{
    std::size_t operator()(const States& s) const
    {
        std::size_t hashValue = 0;
        for (size_t elem : s)
        {
            hashValue ^= std::hash<size_t>{}(elem) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
        }
        return hashValue;
    }
};

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
                break;
            }
        }
    }

    dfa->SetFinalStates(std::move(finishStates));

    return dfa;
}

DFAPtr DFABuilder::BuildFromAST(const ASTPtr& ast)
{
    DFAPtr dfa = DFA::Instance();
    std::queue<States> q;
    int stateCount = 0;
    std::map<States, DFAStatePtr> stateMap;

    const States& startSet = ast->GetRoot()->GetFirstPos();
    DFAStatePtr startState = dfa->CreateState(stateCount++);
    dfa->SetStart(startState);
    stateMap[startSet] = startState;
    q.push(startSet);

    const States& lastPosRoot = ast->GetRoot()->GetLastPos();
    const std::map<int, std::string>& posToChar = ast->GetPosToChar();
    const std::map<int, States>& followPos = ast->GetFollowPos();
    while (!q.empty())
    {
        States current = q.front();
        q.pop();
        DFAStatePtr currentState = stateMap[current];

        std::map<std::string, States> transitions;
        for (size_t pos : current)
        {
            auto itPos = posToChar.find(pos);
            if (itPos != posToChar.end())
            {
                const std::string& symbol = itPos->second;
                if (!transitions.count(symbol))
                    transitions[symbol] = {};

                auto itFollow = followPos.find(pos);
                if (itFollow != followPos.end())
                {
                    transitions[symbol].insert(itFollow->second.cbegin(), itFollow->second.cend());
                }
            }
        }

        for (const auto& [symbol, newSet] : transitions)
        {
            DFAStatePtr newState;
            if (stateMap.find(newSet) == stateMap.end())
            {
                newState = dfa->CreateState(stateCount++);
                stateMap[newSet] = newState;
                q.push(newSet);
            }
            else
            {
                newState = stateMap[newSet];
            }
            currentState->m_transitions[symbol] = newState;
        }

        bool noTransitions = currentState->m_transitions.empty();
        bool onlySelfLoop = (currentState->m_transitions.size() == 1) &&
                            (currentState->m_transitions.begin()->second == currentState);

        if (noTransitions || onlySelfLoop)
        {
            currentState->SetIsFinal(true);
        }
    }


    return dfa;
}
