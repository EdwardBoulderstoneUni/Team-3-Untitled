#include "PlayerController.h"
#include "../Common/Window.h"
#include "../Common/Keyboard.h"
using namespace NCL;
Input PlayerController::get_inputs()
{
    Input input;
    for (int i = 0; i < max_input; i++) {
        input.buttons[i] = buttons[i];
        buttons[i] = false;
    }
    input.look_direction = look_direction;
    input.movement_direction = movement_direction;

    look_direction = NCL::Maths::Vector2();
    movement_direction= NCL::Maths::Vector2();
    return input;
}

void PlayerController::update(const float dt)
{
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE))
          buttons[jump] = true;
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SHIFT))
          buttons[dash] = true;
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W))
          movement_direction = NCL::Maths::Vector2(0, 1);
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S))
          movement_direction = NCL::Maths::Vector2(0, -1);
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A))
          movement_direction = NCL::Maths::Vector2(-1, 0);
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D))
          movement_direction = NCL::Maths::Vector2(1, 0);
      if (Window::GetMouse()->ButtonDown(MouseButtons::LEFT))
          buttons[attack]=true;

}
