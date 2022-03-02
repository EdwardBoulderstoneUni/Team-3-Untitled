#pragma once
struct Input {
	bool attack = false;
	float pitch = 0.0f;
	float yaw = 0.0f;
	bool pause = false;
	bool jump = false;
	bool sprint = false;
};

class InputHandler
{
public:
	InputHandler() = default;
	virtual ~InputHandler() = default;
	virtual Input get_inputs() { return {}; }
};

