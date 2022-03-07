#include "ComponentPhysics.h"

void ComponentPhysics::Init()
{
	Transform trans;
	trans.SetPosition(center);
	trans.SetOrientation(orientation);
	phyObj->SetTransform(trans);
	phyObj->SetGravity(useGravity);
	switch (motionType)
	{
	case ComponentPhysics::Static:
		phyObj->setDynaimc(false);
		break;
	case ComponentPhysics::Dynamic:
		phyObj->setDynaimc(true);
		break;
	default:
		break;
	}
	switch (shapeType)
	{
	case ComponentPhysics::Box:
		phyObj->setVolume(new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(size/2.0f)));
		break;
	case ComponentPhysics::Capsule:
		phyObj->setVolume(new PxCapsuleGeometry(size.x/2,size.y));
		break;
	case ComponentPhysics::Sphere:
		phyObj->setVolume(new PxSphereGeometry(size.x));
		break;
	case ComponentPhysics::Cylinder:
		break;
	case ComponentPhysics::Plane:
		break;
	case ComponentPhysics::Mesh:
		break;
	default:
		break;
	}
}

void ComponentPhysics::Update()
{
}
