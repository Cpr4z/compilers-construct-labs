#pragma once

#include "IAutomationVizu.hpp"
#include "Utils.h"
#include "NFA.hpp"

#include <set>

class AutomationVizualizator: public IAutomationVizu
{
public:
    void CreateVizu(const IAutomationPtr&) override;// noexcept(false) ??

private:
    void FromDotToPng(const fs_path& dot); // noexcept(false) ??
    std::string printAutomat(const StatePtr& state, std::set<StateId>& visited);
    void cleanDirs() const;
    [[nodiscard]] fs_path GeneratePath(utils::VizuType::FileType::e) const;

private:
    static size_t m_countDot;
    static size_t m_countPng;
};
