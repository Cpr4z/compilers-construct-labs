#include "IAutomation.hpp"
#include "NFA.hpp"

class DFA: public IAutomation
{
public:
    ~DFA() = default;
    DFA(const NFAPtr&);
    bool Imitate(const std::string& regex) override;


private:
};
