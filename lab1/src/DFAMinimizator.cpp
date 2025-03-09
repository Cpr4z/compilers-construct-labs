#include "DFAMinimizator.hpp"
#include <queue>
#include <ranges>
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

//    for (const DFAStatePtr& state : dfaStates)
//    {
//        for (const auto& [symbol, nextState] : state->m_transitions)
//        {
//            // Проверяем, существует ли уже символ в списке входящих ребер
//            if (!reverseTransitions.contains(nextState))
//            {
//                reverseTransitions[nextState] = {};
//            }
//            if (!reverseTransitions[nextState].contains(symbol))
//            {
//                reverseTransitions[nextState][symbol] = {};
//            }
//
//            // Добавляем текущее состояние как "обратное" для nextState
//            reverseTransitions[nextState][symbol].push_back(state);
//
//            // Вывод для отладки
//            std::cout << "Adding reverse edge: "
//                      << state->m_id << " --(" << symbol << ")--> " << nextState->m_id << "\n";
//        }
//    }

    std::vector<bool> isTerminal(n, false);
    for (const auto& state : dfa->GetFinalStates())
    {
        isTerminal[dfa->GetStateIndex(state)] = true;
    }

//    std::vector<bool> isTerminal(n, false);
//    std::queue<DFAStatePtr> queue;
//
//    DFAStatePtr startState = dfa->GetStart();
//    isTerminal[dfa->GetStateIndex(startState)] = true;
//    queue.push(startState);
//
//    while (!queue.empty()) {
//        DFAStatePtr current = queue.front();
//        queue.pop();
//
//        for (const auto& [symbol, nextState] : current->m_transitions) {
//            size_t nextIndex = dfa->GetStateIndex(nextState);
//            if (!isTerminal[nextIndex]) {
//                isTerminal[nextIndex] = true;
//                queue.push(nextState);
//            }
//        }
//    }

    auto marked = buildTable(dfa, isTerminal, reverseTransitions);

    std::vector<DFAStatePtr> dfaVector = Utils::Transformation::ToVector(dfaStates);
    std::vector<int> component(n , -1);
    std::map<DFAStatePtr, StateId> indexTable = buildIndexTable(dfa);

//`    for (size_t i = 0; i < n; i++)
//    {
//        if (!marked[0][i])
//        {
//            component[i] = 0;
//        }
//    }`

    int componentCount = 0;
    for (size_t i = 1; i < n; i++)
    {
//        if (isTerminal[i]) continue;

        if (component[i] == -1)
        {
            component[i] = ++componentCount;
            for (size_t j = i + 1; j < n; j++)
            {
                if (!marked[i][j])
                {
                    component[j] = componentCount;
                }
            }
        }
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (!marked[i][j] && component[j] == -1) {
                component[j] = component[i];
            }
        }
    }

    DFAPtr minimizedDFA = DFA::Instance();
    std::map<StateId, DFAStatePtr> newStates;


//    for (size_t i = 0; i < n; i++)
//    {
//        if (!newStates.contains(component[i]))
//        {
//            newStates[component[i]] = minimizedDFA->CreateState(component[i], dfaVector[i]->m_isFinal);
//        }
//    }
//
//    for (size_t i = 0; i < n; i++)
//    {
//        const DFAStatePtr& oldState = dfaVector[i];
//        DFAStatePtr& newState = newStates[component[i]];
//
//        for (const auto& [symbol, nextState] : oldState->m_transitions)
//        {
//            int nextComponent = component[indexTable[nextState]];
//            newState->m_transitions[symbol] = newStates[nextComponent];
//        }
//    }
//    minimizedDFA->SetStart(newStates[component[indexTable[dfa->GetStart()]]]);

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

    // 2. Перенос переходов
    std::map<int, std::map<std::string, int>> newTransitions;
    for (const auto& [oldState, idx] : indexTable) {
        int fromComponent = component[idx];

        for (const auto& [symbol, nextState] : oldState->m_transitions) {
            int toComponent = component[indexTable[nextState]];
            newTransitions[fromComponent][symbol] = toComponent;
        }
    }

    // 3. Применяем переходы к новым состояниям
    for (const auto& [fromState, transitions] : newTransitions) {
        for (const auto& [symbol, toState] : transitions) {
            newStates[fromState]->m_transitions[symbol] = newStates[toState];
        }
    }

    // 4. Установка стартового состояния
    int startComponent = component[indexTable[dfa->GetStart()]];
    minimizedDFA->SetStart(newStates[startComponent]);

    // 5. Установка финальных состояний
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
//            if (!marked[i][j] && (isTerminal[i] != isTerminal[j]))
            if (isTerminal[i] != isTerminal[j])
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
    std::map<DFAStatePtr, DFAStatePtr> stateMapping;
    for (const auto& state : dfa->GetStates()) {
        stateMapping[state] = reversedDFA->CreateState(state->m_id, state->m_isFinal);
    }

    for (const auto& state : dfa->GetStates()) {
        for (const auto& [symbol, nextState] : state->m_transitions) {
            stateMapping[nextState]->m_transitions[symbol] = stateMapping[state];
        }
    }

    std::set<DFAStatePtr> newFinalStates = { stateMapping[dfa->GetStart()] };
    reversedDFA->SetFinalStates(newFinalStates);

    if (!dfa->GetFinalStates().empty()) {
        reversedDFA->SetStart(stateMapping[*dfa->GetFinalStates().begin()]);
    }
    return reversedDFA;
}

DFAPtr DFAMinimizator::BrzozowskiMinimize(const DFAPtr& dfa)
{
    return ReverseDFA(dfa);
}

DFAPtr DFAMinimizator::HopcroftMinimize(const DFAPtr& dfa)
{
    DFAStateSet states = dfa->GetStates();
    std::set<std::string> alphabet = dfa->GetAlphabet();

    std::map<DFAStatePtr, int> stateClass;
    std::vector<std::set<DFAStatePtr>> partitions(2);

    for (const auto& state : states) {
        if (state->m_isFinal) {
            stateClass[state] = 0;
            partitions[0].insert(state);
        } else {
            stateClass[state] = 1;
            partitions[1].insert(state);
        }
    }

    std::queue<std::pair<int, std::string>> workQueue;
    for (const auto& symbol : alphabet) {
        workQueue.emplace(0, symbol);
        workQueue.emplace(1, symbol);
    }

    while (!workQueue.empty()) {
        auto [classIdx, symbol] = workQueue.front();
        workQueue.pop();

        std::map<int, std::set<DFAStatePtr>> involvedStates;
        for (const auto& state : partitions[classIdx]) {
            for (const auto& prevState : states) {
                DFAStatePtr transState = prevState->GetTransition(symbol);
                if (transState && stateClass[transState] == classIdx) {
                    involvedStates[stateClass[prevState]].insert(prevState);
                }
            }
        }

        for (const auto& [oldClass, affectedStates] : involvedStates) {
            if (affectedStates.size() < partitions[oldClass].size()) {
                partitions.emplace_back();
                int newClass = partitions.size() - 1;

                for (const auto& state : affectedStates) {
                    partitions[oldClass].erase(state);
                    partitions[newClass].insert(state);
                    stateClass[state] = newClass;
                }

                if (partitions[newClass].size() > partitions[oldClass].size()) {
                    std::swap(partitions[newClass], partitions[oldClass]);
                }

                for (const auto& sym : alphabet) {
                    workQueue.emplace(newClass, sym);
                }
            }
        }
    }

    DFAPtr minimizedDFA = DFA::Instance();
    std::map<int, DFAStatePtr> newStates;

    for (size_t i = 0; i < partitions.size(); i++) {
        if (!partitions[i].empty()) {
            DFAStatePtr rep = *partitions[i].begin();
            newStates[i] = minimizedDFA->CreateState(rep->m_id, rep->m_isFinal);
        }
    }

    for (size_t i = 0; i < partitions.size(); i++) {
        if (!partitions[i].empty()) {
            DFAStatePtr oldRep = *partitions[i].begin();
            DFAStatePtr newRep = newStates[i];

            for (const auto& [symbol, nextState] : oldRep->m_transitions) {
                int nextClass = stateClass[nextState];
                newRep->m_transitions[symbol] = newStates[nextClass];
            }
        }
    }

    minimizedDFA->SetStart(newStates[stateClass[dfa->GetStart()]]);
    return minimizedDFA;
}
