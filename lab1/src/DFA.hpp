#include "IAutomation.hpp"
#include "NFA.hpp"
#include "DFAState.hpp"

using DFAPtr = std::shared_ptr<class DFA>;
using DFAStateSet = std::set<DFAStatePtr, DFAStateComparator>;

class [[nodiscard]] DFA: public IAutomation
{
public:
    bool Imitate(std::string&& regex) override;

    static DFAPtr Instance();

    DFAStatePtr CreateState(StateId id);
    [[maybe_unused]] DFAStatePtr CreateState(StateId id, bool isFinal);

    // TO DO: move this logic to DFAController
    DFAStateSet GetStates() const;
    std::set<std::string> GetAlphabet() const;
    const std::set<DFAStatePtr>& GetFinalStates() const { return m_finalStates; }
    StateId GetStateIndex(const DFAStatePtr& state) const;
    void SetStart(const DFAStatePtr& start) { m_start = start;};
    DFAStatePtr GetStart() const { return m_start; }
    void SetFinalStates(std::set<DFAStatePtr> finalStates) { m_finalStates = std::move(finalStates); }

private:
    DFAStatePtr m_start;
    std::set<DFAStatePtr> m_finalStates;
};

