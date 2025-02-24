#pragma once

#include <map>
#include <memory>
#include <vector>

using StateId = size_t;
using StatePtr = std::shared_ptr<class State>;

struct State
{
    explicit State(StateId id): m_id(id) {}
    explicit State(StateId id, bool isFinal): m_id(id), m_isFinal(isFinal) {}

    StateId m_id;
    bool m_isFinal = false;
    std::map<std::string, std::vector<StatePtr>> m_transitions;
};

struct StateComparator
{
    bool operator()(const StatePtr& lhs, const StatePtr& rhs) const
    {
        return lhs->m_id < rhs->m_id;
    }
};