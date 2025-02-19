#pragma once

#include "IAutomation.hpp"

#include <set>

using StateId = size_t;
using Token = char32_t;
using StatePtr = std::shared_ptr<class State>;

using NFAPtr = std::shared_ptr<class NFA>;

class NFAImpl;
//class StateComparator;

class NFA : public IAutomation
{
public:
    // IAutomation
    NFA();
    ~NFA();

    bool Imitate(const std::string& regex) override;

    // NFA
    static NFAPtr Instance();
    void Init(const StatePtr& start, const StatePtr& accept);
    void AddTransition(StatePtr&, const StatePtr&, Token);
    StatePtr CreateState(StateId, bool = false);
    std::set<State> GetStates() const;

    StatePtr GetStart() const;
    StatePtr GetAccept() const;

private:
    std::unique_ptr<class NFAImpl> m_impl;
};

using NFAPtr = std::shared_ptr<NFA>;
