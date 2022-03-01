#include "MouseAndKeyboardInput.h"

#include "Window.h"

MouseAndKeyboardInput::MouseAndKeyboardInput() : InputHandler(), keyboard_(NCL::Window::GetKeyboard()),
                                                 mouse_(NCL::Window::GetMouse()) { }

Input MouseAndKeyboardInput::get_inputs()
{
    Input input;
    input.attack = mouse_->ButtonDown(NCL::MouseButtons::LEFT);
	input.pitch = mouse_->GetRelativePosition().y;
    input.yaw = mouse_->GetRelativePosition().x;
    return input;
}
