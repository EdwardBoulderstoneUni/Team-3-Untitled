#include "MouseAndKeyboardInput.h"
#include "Window.h"

MouseAndKeyboardInput::MouseAndKeyboardInput() : UserController(), keyboard_(NCL::Window::GetKeyboard()),
                                                 mouse_(NCL::Window::GetMouse()) { }

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
