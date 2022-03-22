#include "MouseAndKeyboardInput.h"

MouseAndKeyboardInput::MouseAndKeyboardInput(NCL::Mouse* mouse, NCL::Keyboard* keyboard) :
UserController(), keyboard_(keyboard), mouse_(mouse) { }

Input MouseAndKeyboardInput::get_inputs()
{
    Input input;
    input.buttons[attack] = mouse_->ButtonPressed(NCL::MouseButtons::LEFT);
    input.buttons[jump] = keyboard_->KeyPressed(NCL::KeyboardKeys::SPACE);
    input.buttons[pause] = keyboard_->KeyPressed(NCL::KeyboardKeys::ESCAPE);
    input.buttons[dash] = keyboard_->KeyPressed(NCL::KeyboardKeys::SHIFT);
    input.buttons[quit] = keyboard_->KeyPressed(NCL::KeyboardKeys::ESCAPE);
    input.buttons[reload] = keyboard_->KeyPressed(NCL::KeyboardKeys::R);
    input.buttons[grenade] = keyboard_->KeyPressed(NCL::KeyboardKeys::G);
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
