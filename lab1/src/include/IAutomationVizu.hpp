#pragma once
#include <filesystem>
#include <memory>

class IAutomationVizu
{
public:
    virtual ~IAutomationVizu() = 0;
    virtual void Vizualize() = 0;
};

using IAutomationVizuPtr = std::shared_ptr<IAutomationVizu>;
