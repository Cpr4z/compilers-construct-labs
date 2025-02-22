#pragma once

#include <map>
#include <memory>
#include <vector>

using StateId = size_t;
using Token_ = char32_t;

using StatePtr = std::shared_ptr<class State>;

struct State
{
    explicit State(StateId id): m_id(id) {}

    StateId m_id;
    //std::map<Token_, std::vector<std::shared_ptr<State>>> m_transitions;
    std::map<std::string_view, std::vector<StatePtr>> m_transitions;
};