#pragma once
#ifdef _ORBIS
#include "InputBase.h"
#include "PS4Input.h"
#include "UserController.h"
#include <stdexcept>

class ControllerInput final :
    public UserController
{
public:
    ControllerInput(NCL::PS4::PS4Input* controller);
    ~ControllerInput();
    Input get_inputs() override;
    void update(float dt) override;

protected:
    NCL::PS4::PS4Input* controller_;
    
};
#endif