#pragma once
#include "../types/alphabet.hpp"

class Grammar
{
public:
    Grammar() = default;

    explicit Grammar(GrammarTypes::Alphabet non_terminal_symbols,
                     GrammarTypes::Alphabet terminal_symbols,
                     GrammarTypes::ProductionRules production_rules,
                     GrammarTypes::Token start);

    bool operator==(const Grammar& other) const = default;

    friend std::istream& operator>>(std::istream& is, Grammar&);
    friend std::ostream& operator<<(std::ostream& os, const Grammar&);

protected:
    void checkProductionRule(const GrammarTypes::String& from, const GrammarTypes::String& to);
    [[nodiscard]] auto getMergesSymbols() const;
    [[nodiscard]] auto getIntersectedSymbols() const;
    size_t getProductionRulesCount() const;

protected:
    GrammarTypes::Alphabet m_non_terminal_symbols;
    GrammarTypes::Alphabet m_terminal_symbols;
    GrammarTypes::ProductionRules m_production_rules;
    GrammarTypes::Token m_start;
};

