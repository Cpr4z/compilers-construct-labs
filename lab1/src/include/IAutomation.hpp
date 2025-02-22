#pragma once
#include <memory>

using IAutomationPtr = std::shared_ptr<class IAutomation>;

class IAutomation
{
public:
    virtual ~IAutomation() = default;
    virtual bool Imitate(const std::string& regex) = 0;
};


