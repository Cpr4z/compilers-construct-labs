#include "NFA.hpp"

constexpr std::string_view EPSILON = "eps";

NFA::NFA(StatePtr start, StatePtr accept) : m_start(std::move(start)), m_accept(std::move(accept))
{
}

bool NFA::Imitate(const std::string& regex)
{
    // to do

    return true;
}

NFAPtr NFA::CreateBaseAutomat(const std::string& token, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++);
    m_start->m_transitions[token].push_back(m_accept);
    return std::make_shared<NFA>(m_start, m_accept);
}

NFAPtr NFA::CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second)
{
    first->m_accept->m_transitions[EPSILON].push_back(second->m_start);
    return std::make_shared<NFA>(first->m_start, second->m_accept);
}

NFAPtr NFA::CreateKleeneAutomat(const NFAPtr& first, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++);
    m_start->m_transitions[EPSILON].push_back(first->m_start);
    m_start->m_transitions[EPSILON].push_back(m_accept);
    first->m_accept->m_transitions[EPSILON].push_back(m_accept);
    first->m_accept->m_transitions[EPSILON].push_back(first->m_start);

    return std::make_shared<NFA>(m_start, m_accept);
}

NFAPtr NFA::CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id)
{
    m_start = CreateState(id++);
    m_accept = CreateState(id++);
    m_start->m_transitions[EPSILON].push_back(first->m_start);
    m_start->m_transitions[EPSILON].push_back(second->m_start);
    first->m_accept->m_transitions[EPSILON].push_back(m_accept);
    second->m_accept->m_transitions[EPSILON].push_back(m_accept);

    return std::make_shared<NFA>(m_start, m_accept);
}

StatePtr NFA::CreateState(StateId id)
{
    return std::make_shared<State>(id);
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
