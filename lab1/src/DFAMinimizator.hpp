#pragma once

#include "DFA.hpp"
#include <deque>

using DFAMinimizatorPtr = std::shared_ptr<class DFAMinimizator>;

class [[nodiscard]] DFAMinimizator
{
public:
    DFAPtr MinimizeDFA(const DFAPtr& dfa);

    [[nodiscard]] static DFAMinimizatorPtr Instance();

private:
    std::vector<std::vector<bool>> buildTable(const DFAPtr& dfa, const std::vector<bool>& isTerminal,
                                              const std::map<DFAStatePtr, std::map<std::string, std::vector<DFAStatePtr>>>& delta);
    std::map<DFAStatePtr, StateId> buildIndexTable(const DFAPtr& dfa);
};