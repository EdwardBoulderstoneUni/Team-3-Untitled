#include "ComponentCamera.h"

void ComponentCamera::Init() {
	camera->SetDistanceFromObject(30);
	camera->SetAngleAroundObject(0);
	camera->ThirdPersonCamera(gO);
	gO->GetTransform().SetOrientation(camera->GetThirdPersonOrientation());
}

void ComponentCamera::Update(float dt) {
	camera->ThirdPersonCamera(gO);

	//Quaternion orien = gO->GetTransform().GetOrientation();
	//Vector3 vec = gO->GetTransform().GetOrientation().ToEuler();
	//vec.y = camera->GetYaw();
	//orien = orien.EulerAnglesToQuaternion(vec.x, vec.y - 180, vec.z);
	gO->GetTransform().SetOrientation(camera->GetThirdPersonOrientation());
}