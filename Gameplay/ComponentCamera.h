#pragma once
#include "Component.h"
#include "../Common/Camera.h"
class ComponentCamera : public Component {
public:
	void Init() override;
	void Update(float dt) override;

	void ThirdPersonCamera();
private:
	NCL::Camera* camera;

};