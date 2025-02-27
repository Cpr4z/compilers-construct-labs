#pragma once

#include <set>

#include <NFAState.hpp>

#include "IAutomation.hpp"

using StateSet = std::set<NFAStatePtr, NFAStateComparator>;
using NFAPtr = std::shared_ptr<class NFA>;

class NFA : public IAutomation
{
public:
    NFA() = default;
    NFA(NFAStatePtr start, NFAStatePtr accept);

    // IAutomation
    bool Imitate(std::string&& input) override;

    // NFA
    [[nodiscard]] static NFAPtr Instance();
    [[nodiscard]] NFAPtr CreateBaseAutomat(const std::string& token, StateId& id);
    [[nodiscard]] NFAPtr CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second);
    [[nodiscard]] NFAPtr CreateKleeneAutomat(const NFAPtr& first, StateId& id);
    [[nodiscard]] NFAPtr CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id);
    [[nodiscard]] NFAStatePtr CreateState(StateId);
    [[nodiscard]] NFAStatePtr CreateState(StateId, bool);
    [[nodiscard]] NFAStatePtr GetStart() const;
    [[nodiscard]] NFAStatePtr GetAccept() const;

private:
    NFAStatePtr m_start;
    NFAStatePtr m_accept;
};

using NFAWPtr = std::weak_ptr<NFA>;