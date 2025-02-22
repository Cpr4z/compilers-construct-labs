#pragma once

#include "IAutomationVizu.hpp"
#include "Utils.h"
#include "NFA.hpp"

class AutomationVizualizator: public IAutomationVizu
{
public:
    ~AutomationVizualizator();
    void CreateVizu(const IAutomationPtr&) override;// noexcept(false) ??

private:
    void FromDotToPng(const fs_path& dot); // noexcept(false) ??
    void printAutomat(const StatePtr& state, std::set<int>& visited, std::ostream& stream);
    [[nodiscard]] fs_path GeneratePath(utils::VizuType::FileType::e);

private:
    static size_t m_countDot;
    static size_t m_countPng;
};
