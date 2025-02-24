#include "NFA.hpp"

#include <ranges>
#include <algorithm>

#include "MyStack.hpp"

//to do string_view
const std::string EPSILON = "eps";

NFA::NFA(StatePtr start, StatePtr accept) : m_start(std::move(start)), m_accept(std::move(accept))
{
}

bool NFA::Imitate(std::string&& input)
{
    StateSet currentStates(StateComparator{});
    currentStates = GetEpsilonClosure({m_start});

    for (auto&& sv : input | std::views::transform([](char c) {
        return std::string(&c, 1);
    }))
    {
        StateSet nextStates(StateComparator{});
        for (const auto& state : currentStates)
        {
            auto it = state->m_transitions.find(sv);
            if (it != state->m_transitions.end())
            {
                for (const auto& nextState : it->second)
                {
                    nextStates.insert(nextState);
                }
            }
        }

        currentStates = GetEpsilonClosure(nextStates);
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
    m_start->m_transitions[token].push_back(m_accept);
    return std::make_shared<NFA>(m_start, m_accept);
}

NFAPtr NFA::CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second)
{
    first->m_accept->m_transitions[EPSILON].push_back(second->m_start);
    first->m_accept->m_isFinal = false;
    return std::make_shared<NFA>(first->m_start, second->m_accept);
}

NFAPtr NFA::CreateKleeneAutomat(const NFAPtr& first, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++, true);
    m_start->m_transitions[EPSILON].push_back(first->m_start);
    m_start->m_transitions[EPSILON].push_back(m_accept);
    first->m_accept->m_transitions[EPSILON].push_back(m_accept);
    first->m_accept->m_transitions[EPSILON].push_back(first->m_start);
    first->m_accept->m_isFinal = false;
    return std::make_shared<NFA>(m_start, m_accept);
}

NFAPtr NFA::CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++, true);
    m_start->m_transitions[EPSILON].push_back(first->m_start);
    m_start->m_transitions[EPSILON].push_back(second->m_start);
    first->m_accept->m_transitions[EPSILON].push_back(m_accept);
    second->m_accept->m_transitions[EPSILON].push_back(m_accept);
    first->m_accept->m_isFinal = false;
    second->m_accept->m_isFinal = false;
    return std::make_shared<NFA>(m_start, m_accept);
}

StatePtr NFA::CreateState(StateId id)
{
    return std::make_shared<State>(id);
}

StatePtr NFA::CreateState(StateId id, bool isFinal)
{
    return std::make_shared<State>(id, isFinal);
}

NFAPtr NFA::Instance()
{
    return std::make_shared<NFA>();
}

StatePtr NFA::GetStart() const
{
    return m_start;
}

StatePtr NFA::GetAccept() const
{
    return m_accept;
}

StateSet NFA::GetEpsilonClosure(const StateSet& states) const
{
    StateSet closure = states;
    MyStack<StatePtr> stack;

    for (const auto& state: states)
    {
        stack.push(state);
    }

    while (!stack.empty())
    {
        StatePtr state;
        if (stack.pop(state))
        {
            auto it = state->m_transitions.find(EPSILON);
            if (it != state->m_transitions.end())
            {
                for (const auto& nextState : it->second)
                {
                    if (closure.find(nextState) == closure.end())
                    {
                        closure.insert(nextState);
                        stack.push(nextState);
                    }
                }
            }
        }
    }
    return closure;
}
