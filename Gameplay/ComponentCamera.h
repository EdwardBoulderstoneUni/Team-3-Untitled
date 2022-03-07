#pragma once
#include "Component.h"

class ComponentCamera : public Component {
public:
	void Init();
	void Update();

	void ThirdPersonCamera();
private:
	Camera* camera;

};