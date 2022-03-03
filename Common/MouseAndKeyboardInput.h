#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "UserController.h"

class MouseAndKeyboardInput final :
    public UserController
{
public:
    MouseAndKeyboardInput(const NCL::Mouse* mouse, const NCL::Keyboard* keyboard);
    Input get_inputs() override;

protected:
    const NCL::Keyboard* keyboard_;
    const NCL::Mouse* mouse_;
};

