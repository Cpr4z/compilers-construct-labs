#include "IAutomation.hpp"
#include "NFA.hpp"

class DFA: public IAutomation
{
public:
    //DFA(const NFAPtr&);
    ~DFA() = default;
    bool Imitate(const std::string& regex) override;


private:
};

using DFAPtr = std::shared_ptr<DFA>;
