#include "MouseAndKeyboardInput.h"

MouseAndKeyboardInput::MouseAndKeyboardInput(NCL::Mouse* mouse, NCL::Keyboard* keyboard) :
UserController(), keyboard_(keyboard), mouse_(mouse) { }

Input MouseAndKeyboardInput::get_inputs()
{
    Input input;
    input.buttons[attack] = mouse_->ButtonDown(NCL::MouseButtons::LEFT);
    input.buttons[jump] = keyboard_->KeyDown(NCL::KeyboardKeys::SPACE);
    input.buttons[pause] = keyboard_->KeyDown(NCL::KeyboardKeys::ESCAPE);
    input.buttons[sprint] = keyboard_->KeyDown(NCL::KeyboardKeys::SHIFT);
    input.buttons[quit] = keyboard_->KeyDown(NCL::KeyboardKeys::ESCAPE);
    const float movement_y = keyboard_->KeyDown(NCL::KeyboardKeys::W) ? 1.0f : 0 + (keyboard_->KeyDown(NCL::KeyboardKeys::S) ? -1.0f : 0);
    const float movement_x = keyboard_->KeyDown(NCL::KeyboardKeys::A) ? 1.0f : 0 + (keyboard_->KeyDown(NCL::KeyboardKeys::D) ? -1.0f : 0);
    input.movement_direction = NCL::Vector2(movement_x, movement_y);
    input.look_direction = mouse_->GetRelativePosition();

    return input;
}

void MouseAndKeyboardInput::update(const float dt)
{
    keyboard_->UpdateFrameState(dt);
    mouse_->UpdateFrameState(dt);
}
