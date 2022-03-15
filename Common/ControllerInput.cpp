#ifdef _ORBIS
#include "ControllerInput.h"

ControllerInput::ControllerInput(NCL::PS4::PS4Input* controller) :
    UserController(), controller_(controller){}

ControllerInput::~ControllerInput() {}

Input ControllerInput::get_inputs()
{
    Input input;
    input.buttons[attack] = controller_->GetButtonDown(7);
    input.buttons[jump] = controller_->GetButtonDown(1);
    input.buttons[pause] = controller_->GetButtonDown(10);
    input.buttons[sprint] = controller_->GetButtonDown(9);
    input.buttons[quit] = (controller_->GetAxis(2).y == 1.0f) ? true : false ;
    input.movement_direction = NCL::Maths::Vector2(controller_->GetAxis(0).x, controller_->GetAxis(0).y);
    input.look_direction = NCL::Maths::Vector2(controller_->GetAxis(1).x, controller_->GetAxis(1).y);
    return input;
}

void ControllerInput::update(const float dt)
{
}
#endif