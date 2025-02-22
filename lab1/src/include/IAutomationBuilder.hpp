#pragma once
#include <memory>

#include "IAutomation.hpp"

class IAutomationBuilder
{
public:
    virtual ~IAutomationBuilder() = default;
    [[nodiscard]] virtual IAutomationPtr Build() = 0;
};

using IAutomationBuilderPtr = std::shared_ptr<IAutomationBuilder>;


