#include "DFA.hpp"

#include <ranges>

bool DFA::Imitate(std::string&& regex)
{
    DFAStatePtr currentState = m_start;
    for (auto&& symbol: regex | std::views::transform([](char token)
    {
        return std::string{token};
    }))
    {
        if (currentState->m_transitions.count(symbol))
        {
            currentState = currentState->m_transitions[symbol];
//            auto it = currentState->m_transitions.find(symbol);
//            if (it != currentState->m_transitions.end())
//            {
//                currentState = it->second;
//            }
        }
        else
        {
            return false;
        }
    }
    return currentState->m_isFinal;
}

DFAPtr DFA::Instance()
{
    return std::make_shared<DFA>();
}

DFAStatePtr DFA::CreateState(StateId id)
{
    return std::make_shared<DFAState>(id);
}

DFAStatePtr DFA::CreateState(StateId id, bool isFinal)
{
    return std::make_shared<DFAState>(id, isFinal);
}