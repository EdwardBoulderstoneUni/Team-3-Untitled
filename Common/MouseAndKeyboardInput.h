#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "UserController.h"

class MouseAndKeyboardInput final : public UserController
{
public:
    MouseAndKeyboardInput(NCL::Mouse* mouse, NCL::Keyboard* keyboard);
    Input get_inputs() override;
    void update(float dt) override;

protected:
    NCL::Keyboard* keyboard_;
    NCL::Mouse* mouse_;
};

