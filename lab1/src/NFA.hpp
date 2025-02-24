#pragma once

#include <set>

#include <State.hpp>

#include "IAutomation.hpp"

using StateSet = std::set<StatePtr, StateComparator>;
using NFAPtr = std::shared_ptr<class NFA>;

class NFA : public IAutomation
{
public:
    NFA() = default;
    NFA(StatePtr start, StatePtr accept);

    // IAutomation
    bool Imitate(std::string&& input) override;

    // NFA
    [[nodiscard]] static NFAPtr Instance();
    [[nodiscard]] NFAPtr CreateBaseAutomat(const std::string& token, StateId& id);
    [[nodiscard]] NFAPtr CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second);
    [[nodiscard]] NFAPtr CreateKleeneAutomat(const NFAPtr& first, StateId& id);
    [[nodiscard]] NFAPtr CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id);
    [[nodiscard]] StatePtr CreateState(StateId);
    [[nodiscard]] StatePtr CreateState(StateId, bool);
    [[nodiscard]] StatePtr GetStart() const;
    [[nodiscard]] StatePtr GetAccept() const;

private:
    StateSet GetEpsilonClosure(const StateSet&) const;

private:
    StatePtr m_start;
    StatePtr m_accept;
};