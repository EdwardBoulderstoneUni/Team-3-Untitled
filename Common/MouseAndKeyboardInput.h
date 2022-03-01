#pragma once
#include "InputHandler.h"
#include "Keyboard.h"
#include "Mouse.h"

class MouseAndKeyboardInput final :
    public InputHandler
{
public:
    explicit  MouseAndKeyboardInput();
    Input get_inputs() override;

protected:
    const NCL::Keyboard* keyboard_;
    const NCL::Mouse* mouse_;
};

