#pragma once
#include <filesystem>
#include "IAutomation.hpp"

namespace fs = std::filesystem;
using fs_path = fs::path;

class IAutomationVizu
{
public:
    virtual ~IAutomationVizu() = default;
    virtual void CreateVizu(const IAutomationPtr&) = 0;
};

using IAutomationVizuPtr = std::shared_ptr<IAutomationVizu>;
