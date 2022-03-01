#pragma once
#include "InputHandler.h"

class ControllerInput final :
    public InputHandler
{
public:
    explicit ControllerInput() = default;
    Input get_inputs() override
    {
        throw std::runtime_error("Controllers not implemented");
    }
};