#include "ComponentCamera.h"

void ComponentCamera::Init() {
	camera->SetDistanceFromObject(30);
	camera->SetAngleAroundObject(0);
	camera->ThirdPersonCamera(gO);
	gO->GetTransform().SetOrientation(camera->GetThirdPersonOrientation());
}

void ComponentCamera::Update(float dt) {
	camera->ThirdPersonCamera(gO);
	gO->GetTransform().SetOrientation(camera->GetThirdPersonOrientation());
}