#pragma once

#include "DFA.hpp"
#include <deque>

using DFAMinimizatorPtr = std::shared_ptr<class DFAMinimizator>;

class [[nodiscard]] DFAMinimizator
{
public:
    DFAPtr MinimizeDFA(const DFAPtr& dfa);
    DFAPtr BrzhovkiyMinimize(const DFAPtr& dfa);
    DFAPtr MinimizeKhophort(const DFAPtr& dfa);
    DFAPtr MinimizeHopcroft(const DFAPtr& dfa);

    DFAPtr BuildMinimizedFA(const std::vector<std::string>& tokens);

    [[nodiscard]] static DFAMinimizatorPtr Instance();

private:
    DFAPtr ReverseDFA(const DFAPtr& dfa);

    std::vector<std::vector<bool>> buildTable(const DFAPtr& dfa, const std::vector<bool>& isTerminal,
                                              const std::map<DFAStatePtr, std::map<std::string, std::vector<DFAStatePtr>>>& delta);
    std::map<DFAStatePtr, StateId> buildIndexTable(const DFAPtr& dfa);
};