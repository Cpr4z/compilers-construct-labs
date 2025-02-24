#include "AutomationVizualizator.hpp"
#include "NFA.hpp"
#include "DFA.hpp"

#include <fstream>
#include <format>
#include <iostream>

#include <sstream>

using namespace utils;

size_t AutomationVizualizator::m_countDot = 0;
size_t AutomationVizualizator::m_countPng = 0;

void AutomationVizualizator::cleanDirs() const
{
    fs_path dotDir = GeneratePath(VizuType::FileType::DOT);
    fs_path pngDir = GeneratePath(VizuType::FileType::PNG);

    auto deleteAllFiles = [](const fs_path& path){
        if (fs::exists(path) && fs::is_directory(path))
        {
            fs::remove_all(path);
        }
    };

    for (const auto& dir : {dotDir, pngDir})
    {
        deleteAllFiles(dir);
    }
}

void AutomationVizualizator::CreateVizu(const IAutomationPtr& machine)
{
    cleanDirs();
    fs_path path = GeneratePath(VizuType::FileType::DOT);
    std::string_view ext = utils::VizuType::getInfo(VizuType::FileType::DOT, VizuType::InfoType::EXT);
    fs_path targetPath;
    if (const NFAPtr& nfa = std::dynamic_pointer_cast<NFA>(machine))
    {
        targetPath = std::format("{}/{}{}{}", path.string(), AutomationType::getFileNameByType(AutomationType::NFA), ++m_countDot, ext);
        std::ofstream file(targetPath);
        if (!file.is_open())
        {
            return;
        }

        file << "digraph FiniteStateMachine {";
        file << "    rankdir=LR;";
        file << "    node [shape=circle];";

        StatePtr start = nfa->GetStart();
        file << "  start [shape=none, label=\"start\"];\n";
        file << "  start -> " << start->m_id << " [style=dashed];\n";

        std::set<StateId> visited;
        file << printAutomat(start, visited);

        StatePtr accept = nfa->GetAccept();
        file << "   "<< accept->m_id  << " [shape=doublecircle];\n";

        file << "}\n";
        file.close();

        FromDotToPng(targetPath);
    }
    else if (const DFAPtr& dfa = std::dynamic_pointer_cast<DFA>(machine))
    {
        targetPath = std::format("{}{}{}{}", path.string(), AutomationType::getFileNameByType(AutomationType::DFA), ++m_countPng, ext);
    }
    else
    {
        return;
    }
}

void AutomationVizualizator::FromDotToPng(const fs_path& dot)
{
    fs_path path = GeneratePath(VizuType::FileType::PNG);
    std::string_view ext = utils::VizuType::getInfo(VizuType::FileType::PNG, VizuType::InfoType::EXT);
    fs_path targetPath = std::format("{}/{}{}{}", path.string(), AutomationType::getFileNameByType(AutomationType::NFA), ++m_countPng, ext);
    std::ofstream file(targetPath);

    if (!file.is_open())
    {
        throw std::runtime_error("Error while attempting to open the file!");
    }

    try
    {
        std::string command = std::format("dot -Tpng {} -o {}", dot.string(), targetPath.string());
        if (std::system(command.c_str()) == -1)
        {
            throw std::runtime_error("Error occured at the time of executing command!");
        }
    }
    catch (const std::runtime_error& ex)
    {
        file.close();
        throw ex;
    }
    file.close();
}

fs_path AutomationVizualizator::GeneratePath(VizuType::FileType::e type) const
{
    fs_path currentPath = std::filesystem::current_path();
    std::string_view dir = VizuType::getInfo(type, VizuType::InfoType::DIR);
    fs_path parentDir = currentPath.parent_path();
    fs_path targetDir = parentDir / dir;
    if (!fs::exists(targetDir))
    {
        fs::create_directory(targetDir);
    }
    return targetDir;
}

std::string AutomationVizualizator::printAutomat(const StatePtr& state, std::set<StateId>& visited)
{
    if (visited.contains(state->m_id))
    {
        return {};
    }
    visited.insert(state->m_id);
    std::ostringstream out;
    for (auto& [symbol, nextStates]: state->m_transitions)
    {
        for (StatePtr& nextState: nextStates)
        {
            out << "    " << state->m_id << " -> " << nextState->m_id
             << " [label=\"" << symbol << "\"];\n";
            out << printAutomat(nextState, visited);
        }
    }
    return out.str();
}

