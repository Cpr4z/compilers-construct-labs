#pragma once

#include "IAutomation.hpp"
#include <State.hpp>

using NFAPtr = std::shared_ptr<class NFA>;

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