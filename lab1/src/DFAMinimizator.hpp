#pragma once

#include "DFA.hpp"
#include <deque>

class DFAMinimizator
{
public:
    DFAPtr MinimizeDFA(const DFAPtr& dfa);

private:
    std::deque<std::deque<bool>> buildTable(const DFAPtr& dfa);
};