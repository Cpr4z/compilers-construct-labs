#include "IAutomation.hpp"
#include "NFA.hpp"

#include "DFAState.hpp"

using DFAPtr = std::shared_ptr<class DFA>;

class DFA: public IAutomation
{
public:
    bool Imitate(std::string&& regex) override;

    [[nodiscard]] static DFAPtr Instance();

    [[nodiscard]] DFAStatePtr CreateState(StateId id);
    [[nodiscard]] DFAStatePtr CreateState(StateId id, bool isFinal);

    void SetStart(const DFAStatePtr& start) { m_start = start;};
    [[nodiscard]] DFAStatePtr GetStart() const { return m_start; }

private:
    DFAStatePtr m_start;
};

