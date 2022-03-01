#include "PhysicsXObject.h"
#include "../CSC8503Common/Transform.h"
#include "../CSC8503Common/CollisionVolume.h"

using namespace NCL;
using namespace CSC8503;
PhysicsXObject::PhysicsXObject(PxTransform& _transform, PxShape& _volume):
	transform(_transform),volume(_volume)
{

}
PhysicsXObject::~PhysicsXObject()
{
}

