#pragma once
#include <memory>

class IAutomation
{
public:
    virtual ~IAutomation() = default;
    virtual bool Imitate(std::string&& input) = 0;
};

using IAutomationPtr = std::shared_ptr<IAutomation>;

