#pragma once
#include "UserController.h"
#include "PS4Input.h"
#include <stdexcept>

class ControllerInput final :
    public UserController
{
public:
    ControllerInput(NCL::PS4::PS4Input* controller);
    Input get_inputs() override;
    void update(float dt) override;

protected:
    NCL::PS4::PS4Input* controller_;
};