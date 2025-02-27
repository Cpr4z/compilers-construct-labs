#include "DFA.hpp"

bool DFA::Imitate(std::string&& regex)
{
    return true;
}

DFAPtr DFA::Instance()
{
    return std::make_shared<DFA>();
}

DFAStatePtr DFA::CreateState(StateId id)
{
    return std::make_shared<DFAState>(id);
}

DFAStatePtr DFA::CreateState(StateId id, bool isFinal)
{
    return std::make_shared<DFAState>(id, isFinal);
}