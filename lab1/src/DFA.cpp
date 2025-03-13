#include "DFA.hpp"

#include <ranges>
#include <queue>

bool DFA::Imitate(std::string&& regex)
{
    DFAStatePtr currentState = m_start;
    for (auto&& symbol: regex | std::views::transform([](char token)
    {
        return std::string{token};
    }))
    {
        if (!currentState->m_transitions.count(symbol))
        {
            return false;
        }
        currentState = currentState->m_transitions[symbol];
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

DFAStateSet DFA::GetStates() const
{
    DFAStateSet result;
    std::set<StateId> visited;
    std::queue<DFAStatePtr> queue;

    visited.insert(m_start->m_id);
    queue.push(m_start);
    result.insert(m_start);
    while (!queue.empty())
    {
        const DFAStatePtr& currentState = queue.front();
        queue.pop();
        for (const auto& [_, nextState] : currentState->m_transitions)
        {
            if (!visited.contains(nextState->m_id))
            {
                visited.insert(nextState->m_id);
                queue.push(nextState);
                result.insert(nextState);
            }
        }
    }
    return result;
}

StateId DFA::GetStateIndex(const DFAStatePtr& state) const
{
    DFAStateSet states = GetStates();
    auto it = std::ranges::find_if(states, [&state](const auto& item)
    {
        return item == state;
    });

    if (it != states.end())
    {
        return it->get()->m_id;
    }
    return 0;
}

std::set<std::string> DFA::GetAlphabet() const
{
    DFAStateSet dfaStates = GetStates();
    std::set<std::string> result;
    for (const DFAStatePtr& state : dfaStates)
    {
        for (const auto& [symbol, nextState] : state->m_transitions)
        {
            if (!result.contains(symbol))
            {
                result.insert(symbol);
            }
        }
    }
    return result;
}