#pragma once

#include "IAutomationVizu.hpp"
#include "Utils.h"
#include "NFAState.hpp"
#include "DFAState.hpp"
//#include "NFA.hpp"

#include <set>

class AutomationVizualizator: public IAutomationVizu
{
public:
    void CreateVizu(const IAutomationPtr&) override;// noexcept(false) ??

private:
    void FromDotToPng(const fs_path& dot, Utils::AutomationType::e); // noexcept(false) ??
    std::string printAutomat(const NFAStatePtr& state, std::set<StateId>& visited);
    std::string printAutomat(const DFAStatePtr& state, std::set<StateId>& visited);
    void cleanDirs() const;
    [[nodiscard]] fs_path GeneratePath(Utils::VizuType::FileType::e) const;

private:
    static size_t m_countDot;
    static size_t m_countPng;
};
