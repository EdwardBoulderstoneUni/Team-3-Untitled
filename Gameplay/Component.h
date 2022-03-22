#pragma once

class Component {
public:
	enum ComponentType {
		Camera,
		Input,
		Physics,
		Render,
		Paint,
		MAX_COM_TYPE
	};
	virtual void Init() {}
	virtual void Update(float dt) {}
	ComponentType type;
};
