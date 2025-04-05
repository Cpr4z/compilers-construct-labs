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
            new_production_rules[{A1}].insert({GrammarTypes::Token{GrammarTypes::kEpsilon}});
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
    GrammarTypes::Alphabet reachable;
    std::queue<GrammarTypes::Token> queue;
    reachable.insert(m_start);
    queue.push(m_start);
    while (!queue.empty())
    {
        const GrammarTypes::Token current = queue.front();
        queue.pop();
        for (const auto& [lhs, productions] : m_production_rules)
        {
            if (lhs.empty() || lhs.front() != current)
                continue;

            for (const auto& production : productions)
            {
                for (const auto& symbol : production)
                {
                    if (reachable.insert(symbol).second)
                    {
                        queue.push(symbol);
                    }
                }
            }
        }
    }

    for (auto nt_it = m_non_terminal_symbols.begin(); nt_it != m_non_terminal_symbols.end(); )
    {
        if (!reachable.contains(*nt_it))
            nt_it = m_non_terminal_symbols.erase(nt_it);
        else
            ++nt_it;
    }

    for (auto t_it = m_terminal_symbols.begin(); t_it != m_terminal_symbols.end(); )
    {
        if (!reachable.contains(*t_it))
            t_it = m_terminal_symbols.erase(t_it);
        else
            ++t_it;
    }

    for (auto prod_it = m_production_rules.begin(); prod_it != m_production_rules.end(); )
    {
        const auto& lhs = prod_it->first;
        if (lhs.empty() || !reachable.contains(lhs.front()))
        {
            prod_it = m_production_rules.erase(prod_it);
            continue;
        }

        auto& rhs_set = prod_it->second;
        for (auto rhs_it = rhs_set.begin(); rhs_it != rhs_set.end(); )
        {
            bool all_reachable = std::all_of(rhs_it->begin(), rhs_it->end(),
                                             [&](const GrammarTypes::Token& token) {
                                                 return reachable.contains(token);
                                             });
            if (!all_reachable)
                rhs_it = rhs_set.erase(rhs_it);
            else
                ++rhs_it;
        }

        if (rhs_set.empty())
            prod_it = m_production_rules.erase(prod_it);
        else
            ++prod_it;
    }
}

