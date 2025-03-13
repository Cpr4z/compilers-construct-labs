#pragma once
#include <set>
#include "IAutomation.hpp"
#include "NFAState.hpp"

using NFAStateSet = std::set<NFAStatePtr, NFAStateComparator>;
using NFAPtr = std::shared_ptr<class NFA>;

class [[nodiscard]] NFA : public IAutomation
{
public:
    NFA() = default;
    NFA(NFAStatePtr start, NFAStatePtr accept);

    // IAutomation
    bool Imitate(std::string&& input) override;

    // NFA
    static NFAPtr Instance();
    NFAPtr CreateBaseAutomat(const std::string& token, StateId& id);
    NFAPtr CreateConcatAutomat(const NFAPtr& first, const NFAPtr& second);
    NFAPtr CreateKleeneAutomat(const NFAPtr& first, StateId& id);
    NFAPtr CreateAlternateAutomat(const NFAPtr& first, const NFAPtr& second, StateId& id);
    NFAStatePtr CreateState(StateId);
    [[maybe_unused]] NFAStatePtr CreateState(StateId, bool);
    NFAStatePtr GetStart() const;
    NFAStatePtr GetAccept() const;

private:
    NFAStatePtr m_start;
    NFAStatePtr m_accept;
};

using NFAWPtr = std::weak_ptr<NFA>;