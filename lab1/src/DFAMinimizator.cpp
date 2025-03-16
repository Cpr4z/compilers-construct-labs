#include "DFAMinimizator.hpp"
#include <queue>
#include <ranges>
#include <unordered_map>
#include "Utils.h"

DFAMinimizatorPtr DFAMinimizator::Instance()
{
    return std::make_shared<DFAMinimizator>();
}

DFAPtr DFAMinimizator::MinimizeDFA(const DFAPtr& dfa)
{
    DFAStateSet dfaStates = dfa->GetStates();
    size_t n = dfaStates.size();
    std::map<DFAStatePtr, std::map<std::string, std::vector<DFAStatePtr>>> reverseTransitions;
    for (const DFAStatePtr& state : dfaStates)
    {
        for (const auto& [symbol, nextState] : state->m_transitions)
        {
            reverseTransitions[nextState][symbol].push_back(state);
        }
    }

    std::vector<bool> isTerminal(n, false);
    std::queue<DFAStatePtr> queue;

    DFAStatePtr startState = dfa->GetStart();
    isTerminal[dfa->GetStateIndex(startState)] = true;
    queue.push(startState);

    while (!queue.empty()) {
        DFAStatePtr current = queue.front();
        queue.pop();

        for (const auto& [symbol, nextState] : current->m_transitions) {
            size_t nextIndex = dfa->GetStateIndex(nextState);
            if (!isTerminal[nextIndex]) {
                isTerminal[nextIndex] = true;
                queue.push(nextState);
            }
        }
    }

    auto marked = buildTable(dfa, isTerminal, reverseTransitions);

    std::vector<DFAStatePtr> dfaVector = Utils::Transformation::ToVector(dfaStates);
    std::vector<int> component(n , -1);
    std::map<DFAStatePtr, StateId> indexTable = buildIndexTable(dfa);

    for (size_t i = 0; i < n; i++)
    {
        if (!marked[0][i])
        {
            component[i] = 0;
        }
    }

    int componentCount = 0;
    for (size_t i = 1; i < n; i++)
    {
        if (component[i] == -1)
        {
            component[i] = ++componentCount;
            for (size_t j = i + 1; j < n; j++)
            {
                if (!marked[i][j])
                {
                    component[j] = component[i];
                }
            }
        }
    }

    DFAPtr minimizedDFA = DFA::Instance();
    std::map<StateId, DFAStatePtr> newStates;

    for (size_t i = 0; i < n; i++)
    {
        if (!newStates.contains(component[i]))
        {
            newStates[component[i]] = minimizedDFA->CreateState(component[i], dfaVector[i]->m_isFinal);
        }
    }

    for (const auto& [oldState, idx] : indexTable) {
        int newComponent = component[idx];
        if (newComponent == -1)
        {
            component[idx] = 0;
            newComponent = 0;
        }
        if (!newStates.contains(newComponent)) {
            newStates[newComponent] = minimizedDFA->CreateState(newComponent, oldState->m_isFinal);
        }
    }

    std::map<int, std::map<std::string, int>> newTransitions;
    for (const auto& [oldState, idx] : indexTable) {
        int fromComponent = component[idx];

        for (const auto& [symbol, nextState] : oldState->m_transitions) {
            int toComponent = component[indexTable[nextState]];
            newTransitions[fromComponent][symbol] = toComponent;
        }
    }

    for (const auto& [fromState, transitions] : newTransitions) {
        for (const auto& [symbol, toState] : transitions) {
            newStates[fromState]->m_transitions[symbol] = newStates[toState];
        }
    }

    int startComponent = component[indexTable[dfa->GetStart()]];
    minimizedDFA->SetStart(newStates[startComponent]);

    std::set<DFAStatePtr> finalStates;
    for (const auto& state : dfa->GetFinalStates()) {
        finalStates.insert(newStates[component[indexTable[state]]]);
    }
    minimizedDFA->SetFinalStates(finalStates);

    return minimizedDFA;
}

std::map<DFAStatePtr, StateId> DFAMinimizator::buildIndexTable(const DFAPtr& dfa)
{
    std::map<DFAStatePtr, StateId> result;
    DFAStateSet allDFAStates = dfa->GetStates();
    size_t index = 0;
    for (const auto& state: allDFAStates)
    {
        result[state] = index++;
    }
    return result;
}

std::vector<std::vector<bool>> DFAMinimizator::buildTable(const DFAPtr& dfa, const std::vector<bool>& isTerminal,
                                                          const std::map<DFAStatePtr, std::map<std::string, std::vector<DFAStatePtr>>>& deltaInv)
{
    DFAStateSet allDFAStates = dfa->GetStates();
    std::vector<DFAStatePtr> states = Utils::Transformation::ToVector(allDFAStates);
    size_t size = allDFAStates.size();
    std::vector<std::vector<bool>> marked(size, std::vector<bool>(size, false));
    std::queue<std::pair<StateId , StateId>> queue;
    std::map<DFAStatePtr, StateId> indexTable = buildIndexTable(dfa);

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = i + 1; j < size; j++)
        {
            if (!marked[i][j] && (isTerminal[i] != isTerminal[j]))
            {
                marked[i][j] = marked[j][i] = true;
                queue.emplace(i, j);
            }
        }
    }

    while (!queue.empty()) {
        auto [u, v] = queue.front();
        queue.pop();
        for (const auto& symbol: dfa->GetAlphabet())
        {
            if (!deltaInv.contains(states[u]) || !deltaInv.contains(states[v]))
                continue;
            const auto& reachableFromU = deltaInv.at(states[u]);
            const auto& reachableFromV = deltaInv.at(states[v]);
            if (!reachableFromU.contains(symbol) || !reachableFromV.contains(symbol))
                continue;

            for (const DFAStatePtr& r : reachableFromU.at(symbol))
            {
                for (const DFAStatePtr& s : reachableFromV.at(symbol))
                {
                    if (!indexTable.contains(r) || !indexTable.contains(s))
                        continue;
                    size_t rIndex = indexTable[r];
                    size_t sIndex = indexTable[s];

                    if (rIndex > sIndex)
                    {
                        std::swap(rIndex, sIndex);
                    }

                    if (!marked[rIndex][sIndex])
                    {
                        marked[rIndex][sIndex] = marked[sIndex][rIndex] = true;
                        queue.emplace(rIndex, sIndex);
                    }
                }
            }
        }
    }
    return marked;
}

DFAPtr DFAMinimizator::ReverseDFA(const DFAPtr& dfa)
{
    DFAPtr reversedDFA = DFA::Instance();
    std::map<DFAStatePtr, DFAStatePtr> stateMap;

    for (const auto& state : dfa->GetStates()) {
        stateMap[state] = reversedDFA->CreateState(state->m_id, false);
    }

    for (const auto& state : dfa->GetStates()) {
        for (const auto& [symbol, nextState] : state->m_transitions) {
            stateMap[nextState]->m_transitions[symbol] = stateMap[state];
        }
    }

    reversedDFA->SetFinalStates({stateMap[dfa->GetStart()]});
    for (const auto& finalState : dfa->GetFinalStates()) {
        reversedDFA->SetStart(stateMap[finalState]);
    }

    return reversedDFA;
}

DFAPtr DFAMinimizator::BrzhovkiyMinimize(const DFAPtr& dfa)
{
    DFAPtr reversedDFA = ReverseDFA(dfa);
    DFAPtr minimizedDFA = ReverseDFA(reversedDFA);
    return minimizedDFA;
}

DFAPtr DFAMinimizator::BuildMinimizedFA(const std::vector<std::string>& tokens)
{
    return {};
}

DFAPtr DFAMinimizator::MinimizeKhophort(const DFAPtr& dfa)
{
    std::set<DFAStatePtr> finalStates, nonFinalStates;
    for (const DFAStatePtr& state : dfa->GetStates())
    {
        if (state->m_isFinal)
        {
            finalStates.insert(state);
        }
        else
        {
            nonFinalStates.insert(state);
        }
    }

    std::set<std::set<DFAStatePtr>> P;
    if (nonFinalStates.empty())
    {
        P.insert(finalStates);
    }
    else
    {
        P = {finalStates, nonFinalStates};
    }

    std::queue<std::set<DFAStatePtr>> W;
    W.push(finalStates);
    std::set<std::string> alphabet = dfa->GetAlphabet();
    while (!W.empty())
    {
        std::set<DFAStatePtr> classToSplit = W.front();
        W.pop();
        for (const std::string& symbol : alphabet)
        {
            std::set<DFAStatePtr> affectingStates;
            for (const DFAStatePtr& state : dfa->GetStates())
            {
                for (const auto& [transitionSymbol, nextState] : state->m_transitions)
                {
                    if (symbol == transitionSymbol && classToSplit.contains(nextState))
                    {
                        affectingStates.insert(state);
                    }
                }
            }

            for (const std::set<DFAStatePtr>& subset : P)
            {
                std::set<DFAStatePtr> intersection, difference;
                for (const DFAStatePtr& state : subset)
                {
                    if (affectingStates.contains(state))
                    {
                        intersection.insert(state);
                    }
                    else
                    {
                        difference.insert(state);
                    }
                }

                if (!intersection.empty() && !difference.empty())
                {
                    P.erase(subset);
                    P.insert(intersection);
                    P.insert(difference);

                    std::queue<std::set<DFAStatePtr>> Q;
                    bool found = false;
                    while (!W.empty())
                    {
                        auto curr = W.front();
                        W.pop();
                        if (curr == subset)
                        {
                            found = true;
                        }
                        else
                        {
                            Q.push(curr);
                        }
                    }

                    if (found)
                    {
                        Q.push(intersection);
                        Q.push(difference);
                    }
                    else
                    {
                        Q.push(intersection.size() <= difference.size() ? difference : intersection);
                    }
                    W = std::move(Q);
                    break;
                }
            }
        }
    }

    DFAPtr minDFA = DFA::Instance();
    std::map<std::set<DFAStatePtr>, DFAStatePtr> newStates;
    int idCounter = 0;
    for (const std::set<DFAStatePtr>& subset : P)
    {
        DFAStatePtr newState = minDFA->CreateState(idCounter++, false);
        newStates[subset] = newState;
        for (const DFAStatePtr& oldState : subset)
        {
            if (oldState->m_isFinal)
            {
                newState->m_isFinal = true;
                break;
            }
        }
    }

    for (const auto& [oldSet, newSet] : newStates)
    {
        if (!oldSet.empty())
        {
            for (const DFAStatePtr& repr = *oldSet.begin(); const auto& [symbol, targetState] : repr->m_transitions)
            {
                for (const auto& [subset, newTargetSet] : newStates)
                {
                    if (subset.contains(targetState))
                    {
                        newSet->m_transitions.insert({symbol, newTargetSet});
                        break;
                    }
                }
            }
        }
    }

    for (const auto& [oldSet, newSet] : newStates)
    {
        for (const DFAStatePtr& state : oldSet)
        {
            if (state == dfa->GetStart())
            {
                minDFA->SetStart(newStates[oldSet]);
            }
        }
    }
    return minDFA;
}