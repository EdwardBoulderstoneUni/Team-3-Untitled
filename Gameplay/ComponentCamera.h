#pragma once
#include "Component.h"
#include "../Common/Camera.h"
class ComponentCamera : public Component {
public:
	ComponentCamera() { type = ComponentType::Camera; }
	void Init() override;
	void Update(float dt) override;

	NCL::Camera* camera;
	GameObject* gO;
private:
};