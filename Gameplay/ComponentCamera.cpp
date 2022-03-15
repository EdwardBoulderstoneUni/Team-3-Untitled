#include "ComponentCamera.h"
#include "Player.h"

void ComponentCamera::Init() {
	camera->SetDistanceFromObject(30);
	camera->SetAngleAroundObject(0);
	camera->ThirdPersonCamera(gO);
	gO->GetTransform().SetOrientation(camera->GetThirdPersonOrientation());
}

void ComponentCamera::Update(float dt) {
	camera->ThirdPersonCamera(gO);

	//Quaternion orien = gO->GetTransform().GetOrientation();
	//vec.y = camera->GetYaw();
	//orien = orien.EulerAnglesToQuaternion(vec.x, vec.y - 180, vec.z);
	Quaternion orien = camera->GetThirdPersonOrientation();

	gO->GetTransform().SetOrientation(orien);

	Vector3 vec = gO->GetTransform().GetOrientation().ToEuler();
	vec.x = camera->GetPitch();

	Player* player = dynamic_cast<Player*>(gO);
	if (player) {
		player->SetShootDirection(vec);
	}
}