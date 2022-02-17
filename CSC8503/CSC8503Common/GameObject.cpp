#include "GameObject.h"
#include "CollisionDetection.h"
#include "NetworkObject.h"
using namespace NCL::CSC8503;

GameObject::GameObject(string objectName)
{
	name = objectName;
	worldID = -1;
	isActive = true;
	boundingVolume = nullptr;
	physicsObject = nullptr;
	renderObject = nullptr;
}

GameObject::~GameObject()
{
	delete boundingVolume;
	delete physicsObject;
	delete renderObject;
}

bool GameObject::GetBroadphaseAABB(Vector3& outSize) const
{
	if (!boundingVolume)
	{
		return false;
	}
	outSize = broadphaseAABB;
	return true;
}

void GameObject::UpdateBroadphaseAABB()
{
	if (!boundingVolume)
	{
		return;
	}
	if (boundingVolume->type == VolumeType::AABB)
	{
		broadphaseAABB = reinterpret_cast<AABBVolume&>(boundingVolume).GetHalfDimensions();
	}
	else if (boundingVolume->type == VolumeType::Sphere)
	{
		float r = reinterpret_cast<SphereVolume&>(boundingVolume).GetRadius();
		broadphaseAABB = Vector3(r, r, r);
	}
	else if (boundingVolume->type == VolumeType::OBB)
	{
		auto mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		Vector3 halfSizes = reinterpret_cast<OBBVolume&>(boundingVolume).GetHalfDimensions();
		broadphaseAABB = mat * halfSizes;
	}
}

void GameObject::SetNetworkObject(int id) {
	networkObject = new NetworkObject(*this, id);
}
