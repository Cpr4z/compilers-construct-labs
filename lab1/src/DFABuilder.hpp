#pragma once

#include "IAutomationBuilder.hpp"

class DFABuilder: public IAutomationBuilder
{
public:
    DFABuilder() = default;
    ~DFABuilder() = default;
    IAutomationPtr Build() override;
private:

};

using DFABuilderPtr = std::shared_ptr<DFABuilder>;
