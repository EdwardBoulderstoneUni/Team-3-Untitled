#include "ComponentCamera.h"
#include "Player.h"
#define CAMERA_YAW_OFFSET 40.0f
#define CAMERA_OFFSET Vector3(7,6 ,-6)
void ComponentCamera::Init() {
	camera->ThirdPersonCamera(gO, CAMERA_OFFSET, CAMERA_YAW_OFFSET);
}

void ComponentCamera::Update(float dt) {
	camera->ThirdPersonCamera(gO, CAMERA_OFFSET, CAMERA_YAW_OFFSET);
}