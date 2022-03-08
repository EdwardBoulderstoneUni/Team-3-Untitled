#include "ComponentCamera.h"

void ComponentCamera::Init() {
	camera->SetDistanceFromObject(30);
	camera->SetAngleAroundObject(0);
	camera->ThirdPersonCamera(gO);
}

void ComponentCamera::Update(float dt) {
	camera->ThirdPersonCamera(gO);
	
	//camera->SetPosition(gO->GetTransform().GetPosition() - Vector3(0, -10, 30));
}