#include "contextFreeGrammar.hpp"
#include <algorithm>
#include <ranges>
#include <queue>

ContextFreeGrammar::ContextFreeGrammar(GrammarTypes::Alphabet non_terminal_symbols,
                                       GrammarTypes::Alphabet terminal_symbols,
                                       GrammarTypes::ProductionRules production_rules, GrammarTypes::Token start) :
                                       Grammar(std::move(non_terminal_symbols),
                                               std::move(terminal_symbols),
                                               std::move(production_rules),
                                               std::move(start))
                                       {
                                           checkContextFree();
                                       }

ContextFreeGrammar::ContextFreeGrammar(const Grammar& other) : Grammar(other)
{
    checkContextFree();
}

ContextFreeGrammar::ContextFreeGrammar(Grammar&& other) : Grammar(std::move(other))
{
    checkContextFree();
}

void ContextFreeGrammar::checkContextFree()
{
    for (const auto& [from, to] : m_production_rules)
    {
        if (from.size() != 1 || !m_non_terminal_symbols.contains(from.front()))
        {
            throw std::invalid_argument("[ContextFreeGrammar::ContextFreeGrammar] Left part of the production rule can have only one non terminal");
        }
    }
}

void ContextFreeGrammar::eliminateLeftRecursion()
{
    auto new_non_terminal_symbols = m_non_terminal_symbols;
    auto new_production_rules = m_production_rules;
    for (auto i = m_non_terminal_symbols.begin(); i != m_non_terminal_symbols.end(); ++i)
    {
        const auto& Ai = *i;
        for (auto j = m_non_terminal_symbols.begin(); j != i ; ++j)
        {
            const auto& Aj = *j;
            auto Ai_rule = m_production_rules[{Ai}];
            for (const auto& Aj_gamma : m_production_rules[{Ai}])
            {
                if (!(Aj_gamma.size() >= 2 && Aj_gamma.front() == Aj)) // replace
                {
                    continue;
                }
                Ai_rule.erase(Aj_gamma);
                for (const auto& xi : new_production_rules[{Aj}])
                {
                    if (xi.empty())
                    {
                        continue;
                    }
                    auto xi_gamma = xi;
                    xi_gamma.insert(xi_gamma.end(), Aj_gamma.end() + 1, Aj_gamma.end());
                    Ai_rule.insert(xi_gamma);
                }
            }
            m_production_rules[{Ai}] = std::move(Ai_rule);
        }
        eliminateImmediateLeftRecursion_(Ai, new_non_terminal_symbols, new_production_rules);
    }
    m_non_terminal_symbols = std::move(new_non_terminal_symbols);
    m_production_rules = std::move(new_production_rules);
}

void ContextFreeGrammar::eliminateImmediateLeftRecursion_(const GrammarTypes::Token& A,
                                                          GrammarTypes::Alphabet& new_non_terminal_symbols,
                                                          GrammarTypes::ProductionRules& new_production_rules)
{
    if (!hasImmediateLeftRecursionProductionRules(A))
    {
        return;
    }

    auto A1 = A + '\'';
    while (new_non_terminal_symbols.contains(A1))
    {
        A1 = '\'';
    }
    new_non_terminal_symbols.insert(A1);
    new_production_rules[{A}].clear();
    for (const auto& p : m_production_rules[{A}])
    {
        if (p.size() >= 2 && p.front() == A)
        {
            auto alpha = GrammarTypes::String(p.begin() + 1, p.end());
            auto alpha_A1 = alpha;
            alpha_A1.push_back(A1);
            new_production_rules[{A1}].insert(std::move(alpha_A1));
            new_production_rules[{A1}].insert(GrammarTypes::kEpsilon);
        }
        else if (!p.empty() && p.front() != A || p.empty())
        {
            const auto& beta = p;
            auto beta_A1 = beta;
            beta_A1.push_back(A1);
            new_production_rules[{A}].insert(std::move(beta_A1));
        }
    }
}

bool ContextFreeGrammar::hasImmediateLeftRecursionProductionRules(const GrammarTypes::Token &A)
{
    return std::ranges::any_of(m_production_rules[{A}], [&A](const auto& item){
        return item.size() >= 2 && item.front() == A;
    });
}

void ContextFreeGrammar::removeUnreachableSymbols()
{
//    using namespace
    GrammarTypes::Alphabet reachable;
    std::queue<GrammarTypes::Token> queue;
    reachable.insert(m_start);
    queue.push(m_start);
    while (!queue.empty())
    {
        GrammarTypes::Token current = queue.front();
        queue.pop();
        auto it = m_production_rules.find({ current });
        if (it == m_production_rules.end()) continue;
        for (const auto& production : it->second)
        {
            for (const auto& symbol : production)
            {
                if (reachable.insert(symbol).second)
                {
                    queue.push(symbol);
                }
            }
        }

        for (auto it_ = m_non_terminal_symbols.begin(); it_ != m_non_terminal_symbols.end(); )
        {
            if (!reachable.contains(*it_)) {
                it_ = m_non_terminal_symbols.erase(it_);
            } else {
                ++it_;
            }
        }

        for (auto it_ = m_terminal_symbols.begin(); it_ != m_terminal_symbols.end(); ) {
            if (!reachable.contains(*it_)) {
                it_ = m_terminal_symbols.erase(it_);
            } else {
                ++it_;
            }
        }

        for (auto it_ = m_production_rules.begin(); it_ != m_production_rules.end(); )
        {
            if (it_->first.empty() || !reachable.contains(it_->first.front())) {
                it_ = m_production_rules.erase(it_);
                continue;
            }

            auto& rhs_set = it_->second;
            for (auto rhs_it = rhs_set.begin(); rhs_it != rhs_set.end(); )
            {
                bool all_reachable = std::all_of(rhs_it->begin(), rhs_it->end(),
                                                 [&](const auto& t) { return reachable.contains(t); });
                if (!all_reachable)
                {
                    rhs_it = rhs_set.erase(rhs_it);
                }
                else
                {
                    ++rhs_it;
                }
            }

            if (rhs_set.empty())
            {
                it_ = m_production_rules.erase(it_);
            }
            else
            {
                ++it_;
            }
        }
    }
}

