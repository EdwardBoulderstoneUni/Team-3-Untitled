#include "MouseAndKeyboardInput.h"

MouseAndKeyboardInput::MouseAndKeyboardInput(NCL::Mouse* mouse, NCL::Keyboard* keyboard) :
UserController(), keyboard_(keyboard), mouse_(mouse) { }

Input MouseAndKeyboardInput::get_inputs()
{
    Input input;
    input.buttons[attack] = mouse_->ButtonDown(NCL::MouseButtons::LEFT);
	input.pitch = mouse_->GetRelativePosition().y;
    input.yaw = mouse_->GetRelativePosition().x;
    input.buttons[jump] = keyboard_->KeyDown(NCL::KeyboardKeys::SPACE);
    input.buttons[pause] = keyboard_->KeyDown(NCL::KeyboardKeys::ESCAPE);
    input.buttons[sprint] = keyboard_->KeyDown(NCL::KeyboardKeys::SHIFT);
    return input;
}

void MouseAndKeyboardInput::update(const float dt)
{
    keyboard_->UpdateFrameState(dt);
    mouse_->UpdateFrameState(dt);
}
