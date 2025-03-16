#pragma once

#include <map>
#include <memory>
#include <vector>

using StateId = size_t;
using DFAStatePtr = std::shared_ptr<class DFAState>;

struct [[nodiscard]] DFAState
{
    explicit DFAState(StateId id): m_id(id) {}
    explicit DFAState(StateId id, bool isFinal): m_id(id), m_isFinal(isFinal) {}

    DFAStatePtr GetTransition(const std::string& symbol) const
    {
        auto it = m_transitions.find(symbol);
        if (it != m_transitions.end())
        {
            return it->second;
        }
        return {};
    }

    void SetIsFinal(bool val) { m_isFinal = val; }

    StateId m_id = 0;
    bool m_isFinal = false;
    std::map<std::string, DFAStatePtr> m_transitions;
};

struct DFAStateComparator
{
    bool operator()(const DFAStatePtr& lhs, const DFAStatePtr& rhs) const
    {
        return lhs->m_id < rhs->m_id;
    }
};
