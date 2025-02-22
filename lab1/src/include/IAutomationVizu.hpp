#pragma once

#include <filesystem>
#include <boost/filesystem.hpp>
#include <memory>

#include "boost/bimap.hpp"

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
