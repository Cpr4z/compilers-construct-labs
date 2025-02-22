#pragma once

#include "IAutomationBuilder.hpp"

class DFABuilder: public IAutomationBuilder
{
public:
    IAutomationPtr Build() override;
private:

};

using DFABuilderPtr = std::shared_ptr<DFABuilder>;
