#pragma once
#include "UserController.h"
#include <stdexcept>

class ControllerInput final :
    public UserController
{
public:
    explicit ControllerInput() = default;
    Input get_inputs() override
    {
        throw std::runtime_error("Controllers not implemented");
    }
};