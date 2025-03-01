#include "NFA.hpp"

#include <ranges>
#include <algorithm>

#include "Utils.h"

using namespace utils::tokenConstants;

NFA::NFA(NFAStatePtr start, NFAStatePtr accept) : m_start(std::move(start)), m_accept(std::move(accept))
{
}

bool NFA::Imitate(std::string&& input)
{
    StateSet currentStates(NFAStateComparator{});
    currentStates.insert(m_start);
    currentStates = utils::Transformation::GetEpsilonClosure(currentStates);

    for (auto&& str : input | std::views::transform([](char token) {
        return std::string{token};
    }))
    {
        StateSet nextStates(NFAStateComparator{});
        for (const auto& state : currentStates)
        {
            auto it = state->m_transitions.find(str);
            if (it != state->m_transitions.end())
            {
                for (const auto& nextState : it->second)
                {
                    nextStates.insert(nextState);
                }
            }
        }

        currentStates = utils::Transformation::GetEpsilonClosure(nextStates);
        if (currentStates.empty())
        {
            return false;
        }
    }

    return std::ranges::any_of(currentStates, [](const auto& state){return state->m_isFinal;});
}

NFAPtr NFA::CreateBaseAutomat(const std::string& token, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++, true);
    m_start->m_transitions[token].insert(m_accept);
    return std::make_shared<NFA>(m_start, m_accept);
}

NFAPtr NFA::CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second)
{
    first->m_accept->m_transitions[EPSILON].insert(second->m_start);
    first->m_accept->m_isFinal = false;
    return std::make_shared<NFA>(first->m_start, second->m_accept);
}

NFAPtr NFA::CreateKleeneAutomat(const NFAPtr& first, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++, true);
    m_start->m_transitions[EPSILON].insert(first->m_start);
    m_start->m_transitions[EPSILON].insert(m_accept);
    first->m_accept->m_transitions[EPSILON].insert(m_accept);
    first->m_accept->m_transitions[EPSILON].insert(first->m_start);
    first->m_accept->m_isFinal = false;
    return std::make_shared<NFA>(m_start, m_accept);
}

NFAPtr NFA::CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++, true);
    m_start->m_transitions[EPSILON].insert(first->m_start);
    m_start->m_transitions[EPSILON].insert(second->m_start);
    first->m_accept->m_transitions[EPSILON].insert(m_accept);
    second->m_accept->m_transitions[EPSILON].insert(m_accept);
    first->m_accept->m_isFinal = false;
    second->m_accept->m_isFinal = false;
    return std::make_shared<NFA>(m_start, m_accept);
}

NFAStatePtr NFA::CreateState(StateId id)
{
    return std::make_shared<NFAState>(id);
}

NFAStatePtr NFA::CreateState(StateId id, bool isFinal)
{
    return std::make_shared<NFAState>(id, isFinal);
}

NFAPtr NFA::Instance()
{
    return std::make_shared<NFA>();
}

NFAStatePtr NFA::GetStart() const
{
    return m_start;
}

NFAStatePtr NFA::GetAccept() const
{
    return m_accept;
}
