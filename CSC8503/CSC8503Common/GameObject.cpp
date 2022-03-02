#include "GameObject.h"
#include "CollisionDetection.h"
#include "NetworkObject.h"
using namespace NCL::CSC8503;

GameObject::GameObject(string objectName)
{
	name_ = objectName;
	worldID = -1;
	is_active_ = true;
	bounding_volume_ = nullptr;
	physics_object_ = nullptr;
	render_object_ = nullptr;
}

GameObject::GameObject(PhysicsXSystem* physics_system, const VolumeType volume_type, const Vector3& mesh_shape, const Vector3& shape_dimensions, const Vector3& position, const float inverse_mass, const string
                       & name) : GameObject(name)
{
	const auto volume_shape = mesh_shape * shape_dimensions;
	GeometryData geo_data;
	switch (volume_type) {
	case VolumeType::AABB:
		geo_data.type = GeometryData::Box;
		geo_data.data.boxData = GeometryData::Data::BoxData(volume_shape);
		bounding_volume_ = new AABBVolume(volume_shape);
		transform_.SetScale(shape_dimensions);
		break;

	case VolumeType::Sphere:
		bounding_volume_ = new SphereVolume(volume_shape.x);
		transform_.SetScale(shape_dimensions);
		break;

	case VolumeType::Capsule:
		
		bounding_volume_ = new CapsuleVolume(volume_shape.x, volume_shape.y);
		transform_.SetScale(shape_dimensions);
		break;
	default: 
		break;
	}
	transform_.SetPosition(position);

	physics_object_ = new PhysicsObject(&transform_, bounding_volume_);
	physics_object_->SetInverseMass(inverse_mass);
	if (volume_type == VolumeType::AABB)
		physics_object_->InitCubeInertia();
	else
		physics_object_->InitSphereInertia();


	physics_x_object_ = physics_system->createPhysicsXObject(transform_, geo_data);
	if (inverse_mass > 0.0001f)
		physics_system->addDynamicActor(*this);
	else
		physics_system->addStaticActor(*this);
	
}

GameObject::~GameObject()
{
	delete bounding_volume_;
	delete physics_object_;
	delete render_object_;
}

bool GameObject::GetBroadphaseAABB(Vector3& outSize) const
{
	if (!bounding_volume_)
	{
		return false;
	}
	outSize = broadphaseAABB;
	return true;
}

void GameObject::UpdateBroadphaseAABB()
{
	if (!bounding_volume_)
	{
		return;
	}
	if (bounding_volume_->type == VolumeType::AABB)
	{
		broadphaseAABB = reinterpret_cast<AABBVolume&>(bounding_volume_).GetHalfDimensions();
	}
	else if (bounding_volume_->type == VolumeType::Sphere)
	{
		float r = reinterpret_cast<SphereVolume&>(bounding_volume_).GetRadius();
		broadphaseAABB = Vector3(r, r, r);
	}
	else if (bounding_volume_->type == VolumeType::OBB)
	{
		auto mat = Matrix3(transform_.GetOrientation());
		mat = mat.Absolute();
		Vector3 halfSizes = reinterpret_cast<OBBVolume&>(bounding_volume_).GetHalfDimensions();
		broadphaseAABB = mat * halfSizes;
	}
}

void GameObject::SetNetworkObject(int id) {
	network_object_ = new NetworkObject(*this, id);
}