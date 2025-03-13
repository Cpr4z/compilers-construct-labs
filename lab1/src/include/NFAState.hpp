#pragma once
#include <map>
#include <memory>
#include <string>
#include <set>

using StateId = size_t;
using NFAStatePtr = std::shared_ptr<class NFAState>;

struct NFAState
{
    explicit NFAState(StateId id): m_id(id) {}
    explicit NFAState(StateId id, bool isFinal): m_id(id), m_isFinal(isFinal) {}

    StateId m_id = 0;
    bool m_isFinal = false;
    std::map<std::string, std::set<NFAStatePtr, struct NFAStateComparator>> m_transitions;
};

struct NFAStateComparator
{
    bool operator()(const NFAStatePtr& lhs, const NFAStatePtr& rhs) const
    {
        return lhs->m_id < rhs->m_id;
    }
};