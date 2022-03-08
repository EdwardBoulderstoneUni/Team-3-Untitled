#pragma once

class Component {
public:
	enum ComponentType {
		Camera,
		Input,
		Physics,
		Render,
		MAX_COM_TYPE
	};
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	ComponentType type;
};
