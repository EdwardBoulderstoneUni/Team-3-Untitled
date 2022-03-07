#pragma once
#include "Component.h"
#include "../Common/Camera.h"
class ComponentCamera : public Component {
public:
	void Init();
	void Update();

	void ThirdPersonCamera();
private:
	NCL::Camera* camera;

};