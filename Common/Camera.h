#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "../CSC8503/CSC8503Common/GameObject.h"

namespace NCL
{
	using namespace Maths;

	enum class CameraType
	{
		Orthographic,
		Perspective
	};

	class Camera
	{
	public:
		Camera(void)
		{
			left = 0;
			right = 0;
			top = 0;
			bottom = 0;

			pitch = 0.0f;
			yaw = 0.0f;

			fov = 45.0f;
			nearPlane = 1.0f;
			farPlane = 100.0f;

			camType = CameraType::Perspective;
		}

		Camera(float pitch, float yaw, const Vector3& position) : Camera()
		{
			this->pitch = pitch;
			this->yaw = yaw;
			this->position = position;

			this->fov = 45.0f;
			this->nearPlane = 1.0f;
			this->farPlane = 100.0f;
			this->distanceFromObject = 5;
			this->angleAroundObject = 0;

			this->camType = CameraType::Perspective;
		}

		void UpdateCamera(float dt);

		float GetFieldOfVision() const
		{
			return fov;
		}

		float GetNearPlane() const
		{
			return nearPlane;
		}

		float GetFarPlane() const
		{
			return farPlane;
		}

		void SetNearPlane(float val)
		{
			nearPlane = val;
		}

		void SetFarPlane(float val)
		{
			farPlane = val;
		}

		void SetDistanceFromObject(float val)
		{
			distanceFromObject = val;
		}

		float GetAngleAroundObject() const
		{
			return angleAroundObject;
		}

		void SetAngleAroundObject(float val)
		{
			angleAroundObject = val;
		}

		//Builds a view matrix for the current camera variables, suitable for sending straight
		//to a vertex shader (i.e it's already an 'inverse camera matrix').
		Matrix4 BuildViewMatrix() const;

		Matrix4 BuildProjectionMatrix(float currentAspect = 1.0f) const;

		//Gets position in world space
		Vector3 GetPosition() const { return position; }
		//Sets position in world space
		void SetPosition(const Vector3& val) { position = val; }

		//Gets yaw, in degrees
		float GetYaw() const { return yaw; }
		//Sets yaw, in degrees
		void SetYaw(float y) { yaw = y; }

		//Gets pitch, in degrees
		float GetPitch() const { return pitch; }
		//Sets pitch, in degrees
		void SetPitch(float p) { pitch = p; }

		Quaternion GetThirdPersonOrientation() {
			Quaternion orientation;
			orientation = orientation.EulerAnglesToQuaternion(pitch, yaw, 0.0f);
			return orientation;
		}

		static Camera BuildPerspectiveCamera(const Vector3& pos, float pitch, float yaw, float fov, float near,
		                                     float far);
		static Camera BuildOrthoCamera(const Vector3& pos, float pitch, float yaw, float left, float right, float top,
		                               float bottom, float near, float far);

		void ThirdPersonCamera(NCL::CSC8503::GameObject* object,Vector3 offset,float yawoffset);
	protected:
		CameraType camType;

		float nearPlane;
		float farPlane;
		float left;
		float right;
		float top;
		float bottom;

		float fov;
		float yaw;
		float pitch;
		float distanceFromObject;
		float angleAroundObject;
		Vector3 position;
	};
}
