#include "Camera.h"
#include "Maths.h"
#include "Window.h"
#include <algorithm>
#include <iostream>

using namespace NCL;

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float dt)
{
	//Update the mouse by how much
	pitch -= Window::GetInterface()->get_look_direction().y;
	yaw -= Window::GetInterface()->get_look_direction().x;

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}

	float frameSpeed = 100 * dt;

	if (Window::GetInterface()->get_movement().y > 0)
	{
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * frameSpeed;
	}
	if (Window::GetInterface()->get_movement().y < 0)
	{
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * frameSpeed;
	}

	if (Window::GetInterface()->get_movement().x > 0)
	{
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed;
	}
	if (Window::GetInterface()->get_movement().x < 0)
	{
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed;
	}

	if (Window::GetInterface()->button_down(sprint))
	{
		position.y += frameSpeed;
	}
	if (Window::GetInterface()->button_down(jump))
	{
		position.y -= frameSpeed;
	}
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() const
{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};

Matrix4 Camera::BuildProjectionMatrix(float currentAspect) const
{
	if (camType == CameraType::Orthographic)
	{
		return Matrix4::Orthographic(nearPlane, farPlane, right, left, top, bottom);
	}
	//else if (camType == CameraType::Perspective) {
	return Matrix4::Perspective(nearPlane, farPlane, currentAspect, fov);
	//}
}

Camera Camera::BuildPerspectiveCamera(const Vector3& pos, float pitch, float yaw, float fov, float nearPlane,
                                      float farPlane)
{
	Camera c;
	c.camType = CameraType::Perspective;
	c.position = pos;
	c.pitch = pitch;
	c.yaw = yaw;
	c.nearPlane = nearPlane;
	c.farPlane = farPlane;

	c.fov = fov;

	return c;
}

Camera Camera::BuildOrthoCamera(const Vector3& pos, float pitch, float yaw, float left, float right, float top,
                                float bottom, float nearPlane, float farPlane)
{
	Camera c;
	c.camType = CameraType::Orthographic;
	c.position = pos;
	c.pitch = pitch;
	c.yaw = yaw;
	c.nearPlane = nearPlane;
	c.farPlane = farPlane;

	c.left = left;
	c.right = right;
	c.top = top;
	c.bottom = bottom;

	return c;
}

void Camera::ThirdPersonCamera(NCL::CSC8503::GameObject* object) {
	//in this way of control we use left mouse to change pitch and right mouse to change the angle of camera
	if (Window::GetMouse()->ButtonHeld(NCL::MouseButtons::LEFT)) {
		pitch -= (Window::GetMouse()->GetRelativePosition().y);
	}
	if (Window::GetMouse()->ButtonHeld(NCL::MouseButtons::RIGHT)) {
		float angleChange = Window::GetMouse()->GetRelativePosition().x;
		angleAroundObject += angleChange;
	}

	//in this way we can just move the mouse to change the view
	//pitch -= (Window::GetMouse()->GetRelativePosition().y);
	//float angleChange = Window::GetMouse()->GetRelativePosition().x;
	//angleAroundObject += angleChange;

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	//we can just reset the value of angleAroundObject and pitch to reset the view
				//if (Window::GetMouse()->ButtonHeld(NCL::MouseButtons::MIDDLE)) {
	//	angleAroundObject = 0;
	//	pitch = -15;
	//}

	float horizontalDistance = distanceFromObject * cos(DegreesToRadians(pitch));
	float verticalDistance = distanceFromObject * sin(DegreesToRadians(pitch));

	yaw = 180.0f - (object->GetTransform().GetOrientation().ToEuler().y + angleAroundObject);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	float theta = object->GetTransform().GetOrientation().ToEuler().y + angleAroundObject;
	float offsetX = horizontalDistance * sin(DegreesToRadians(theta));
	float offsetZ = horizontalDistance * cos(DegreesToRadians(theta));

	float cameraX = object->GetTransform().GetPosition().x + offsetX;
	float cameraZ = object->GetTransform().GetPosition().z - offsetZ;
	float cameraY = object->GetTransform().GetPosition().y - verticalDistance;

	SetPosition(Vector3(cameraX, cameraY, cameraZ));
	SetPitch(pitch);
	SetYaw(yaw);
}
