#pragma once
#include "grammar.hpp"

class ContextFreeGrammar : public Grammar
{
public:
    explicit ContextFreeGrammar(GrammarTypes::Alphabet non_terminal_symbols,
                                GrammarTypes::Alphabet terminal_symbols,
                                GrammarTypes::ProductionRules production_rules,
                                GrammarTypes::Token start);

    ContextFreeGrammar(const Grammar& other);
    ContextFreeGrammar(Grammar&& other);

    void eliminateLeftRecursion();
    void removeUnreachableSymbols();

private:
    void checkContextFree();
    void eliminateImmediateLeftRecursion_(const GrammarTypes::Token& A,
                                          GrammarTypes::Alphabet& new_non_terminal_symbols,
                                          GrammarTypes::ProductionRules& new_production_rules);
    bool hasImmediateLeftRecursionProductionRules(const GrammarTypes::Token& A);

};
