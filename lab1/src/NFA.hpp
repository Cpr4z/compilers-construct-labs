#pragma once

#include "IAutomation.hpp"

#include <map>
#include <vector>

using StateId = size_t;
using Token = char;

using NFAPtr = std::shared_ptr<class NFA>;

struct State
{
    explicit State(StateId id) : m_id(id){}

    bool operator<(const State& other) const
    {
        return m_id < other.m_id;
    }

    StateId m_id;
    std::map<Token, std::vector<std::shared_ptr<State>>> m_transitions;
};

using StatePtr = std::shared_ptr<State>;


class NFA : public IAutomation
{
public:
    NFA() = default;
    NFA(StatePtr lhs, StatePtr rhs);

    // IAutomation
    bool Imitate(const std::string& regex) override;

    // NFA
    [[nodiscard]] static NFAPtr Instance();
    [[nodiscard]] NFAPtr CreateBaseAutomat(const std::string& token, StateId& id);
    [[nodiscard]] NFAPtr CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second);
    [[nodiscard]] NFAPtr CreateKleeneAutomat(const NFAPtr& first, StateId& id);
    [[nodiscard]] NFAPtr CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id);
    [[nodiscard]] StatePtr CreateState(StateId);
    [[nodiscard]] StatePtr GetStart() const;
    [[nodiscard]] StatePtr GetAccept() const;

private:
    StatePtr m_start;
    StatePtr m_accept;
};

using NFAPtr = std::shared_ptr<NFA>;
