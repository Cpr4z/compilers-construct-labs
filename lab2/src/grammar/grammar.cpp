#include "grammar.hpp"
#include <numeric>
#include "../utils/setUtils.hpp"
#include "../utils/io_utils.hpp"

auto Grammar::getMergesSymbols() const
{
    return Utils::SetUtils::setUnion(m_non_terminal_symbols, m_terminal_symbols);
}

auto Grammar::getIntersectedSymbols() const
{
    return  Utils::SetUtils::setIntersection(m_non_terminal_symbols, m_terminal_symbols);
}

Grammar::Grammar(GrammarTypes::Alphabet non_terminal_symbols,
                 GrammarTypes::Alphabet terminal_symbols,
                 GrammarTypes::ProductionRules production_rules,
                 GrammarTypes::Token start) :
                 m_non_terminal_symbols(std::move(non_terminal_symbols)),
                 m_terminal_symbols(std::move(terminal_symbols)),
                 m_production_rules(std::move(production_rules)),
                 m_start(std::move(start))
{
    if (const auto &intersection = getIntersectedSymbols(); !intersection.empty()) {
        throw std::invalid_argument(
                "[Grammar::Grammar] Σ and N contains common symbols: " + Utils::SetUtils::toString(intersection));
    }

    for (const auto &[from, tos]: m_production_rules) {
        for (const auto &to: tos) {
            checkProductionRule(from, to);
        }
    }

    if (m_start.empty() && m_non_terminal_symbols.empty() ||
        !m_non_terminal_symbols.contains(m_start)) {
        throw std::invalid_argument("[Grammar::Grammar] S must be in N");
    }
}

void Grammar::checkProductionRule(const GrammarTypes::String& from, const GrammarTypes::String& to)
{
    GrammarTypes::String transformedFrom;
    for (const auto& token : from)
    {
        transformedFrom.push_back(token);
    }
    bool isAllNonTerminal = std::ranges::all_of(transformedFrom, [&](const auto& item){ return m_non_terminal_symbols.contains(item) && !m_terminal_symbols.contains(item); });
    if (!isAllNonTerminal)
    {
        throw std::invalid_argument(
                "[Grammar::checkProductionRule_] Symbols in the left part of the production rule must contain non-terminal");
    }

    const auto& mergedSymbols = getMergesSymbols();
    GrammarTypes::String transformedTo;
    for (const auto& token : to)
    {
        for (const char symbol : token)
        {
            transformedTo.push_back(GrammarTypes::Token{symbol});
        }
    }
    bool isBelongAnySymbols = std::ranges::all_of(transformedTo, [&mergedSymbols](const auto& item){return mergedSymbols.contains(item); });

    if (!isBelongAnySymbols)
    {
        //throw std::invalid_argument(
                //"[Grammar::checkProductionRule_] Symbols in the right part of the production rule must be in Σ ∪ N");
    }
}

size_t Grammar::getProductionRulesCount() const
{
    return std::accumulate(m_production_rules.cbegin(), m_production_rules.cend(), 0,
                           [](int sum, const auto& rule){ return sum + rule.second.size();});
}

std::istream& operator>>(std::istream& is, Grammar& grammar)
{
    GrammarTypes::Alphabet non_terminal_symbols;
    GrammarTypes::Alphabet terminal_symbols;
    GrammarTypes::ProductionRules production_rules;
    GrammarTypes::Token start_token;

    size_t n;
    is >> n;

    for (size_t i = 0; i < n; ++i)
    {
        GrammarTypes::Token token;
        is >> token;
        non_terminal_symbols.insert(std::move(token));
    }

    is >> n;

    for (size_t i = 0; i < n; ++i)
    {
        GrammarTypes::Token token;
        is >> token;
        terminal_symbols.insert(std::move(token));
    }

    is >> n;

    for (size_t i = 0; i < n; ++i)
    {
        GrammarTypes::String from, to;
        GrammarTypes::Token token;
        while((is >> token), token != GrammarTypes::Token{Utils::ReaderHelpers::ruleSymbol})
        {
            from.push_back(std::move(token));
        }

        std::string line;
        std::getline(is, line);
        std::stringstream ss{line};

        while(ss >> token)
        {
            to.push_back(std::move(token));
        }
        production_rules[from].insert(std::move(to));
    }

    is >> start_token;
    grammar = Grammar {
        non_terminal_symbols,
        terminal_symbols,
        production_rules,
        start_token
    };

    return is;
}

std::ostream& operator<<(std::ostream& os, const Grammar& grammar)
{
    os << grammar.m_non_terminal_symbols.size() << '\n';
    Utils::IOUtils::printContainer(grammar.m_non_terminal_symbols, os, " ");
    os << '\n' << grammar.m_terminal_symbols.size() << '\n';
    Utils::IOUtils::printContainer(grammar.m_terminal_symbols, os, " ");
    os << '\n' << grammar.getProductionRulesCount() << '\n';
    for (const auto& [from, tos] : grammar.m_production_rules)
    {
        for (const auto& to : tos)
        {
            Utils::IOUtils::printContainer(from, os, " ");
            os << " ";
            os << Utils::ReaderHelpers::ruleSymbol;
            os << " ";
            Utils::IOUtils::printContainer(to, os, " ");
            os << '\n';
        }
    }
    os << grammar.m_start << '\n';
    return os;
}
