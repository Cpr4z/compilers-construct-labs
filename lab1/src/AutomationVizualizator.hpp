#pragma once

#include "IAutomationVizu.hpp"


class AutomationVizualizator: public IAutomationVizu
{
public:
    ~AutomationVizualizator() = default;
    void Vizualize() override;
};
