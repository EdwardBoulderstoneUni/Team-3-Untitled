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

    movement_direction= NCL::Maths::Vector2();
    return input;
}

void PlayerController::update(const float dt)
{
      if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE))
          buttons[jump] = true;
      if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SHIFT))
          buttons[dash] = true;
      if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::R))
          buttons[reload] = true;
	  if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W))
		  movement_direction= NCL::Maths::Vector2(0, 1);
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S))
		  movement_direction= NCL::Maths::Vector2(0, -1);
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A))
		  movement_direction = NCL::Maths::Vector2(-1, 0);
      if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D))
          movement_direction = NCL::Maths::Vector2(1, 0);
      if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT))
          buttons[attack] = true;

       look_direction.x -= Window::GetMouse()->GetRelativePosition().y;
       look_direction.x = std::min(look_direction.x, 90.0f);
       look_direction.x = std::max(look_direction.x, -90.0f);
       look_direction.y -= Window::GetMouse()->GetRelativePosition().x;
       if (look_direction.y < 0)
       {
           look_direction.y += 360.0f;
       }
       if (look_direction.y > 360.0f)
       {
           look_direction.y -= 360.0f;
       }
}
