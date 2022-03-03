#pragma once
enum Buttons
{
	attack,
	pause,
	jump,
	sprint,
	max_input
};

struct Input {
	bool buttons[max_input] = {false};
	float pitch = 0.0f;
	float yaw = 0.0f;
};

class UserController
{
public:
	UserController() = default;
	virtual ~UserController() = default;
	virtual Input get_inputs() { return {}; }
};

