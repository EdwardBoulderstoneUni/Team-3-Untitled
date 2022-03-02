#pragma once
#include"../../include/PhysX/PxPhysicsAPI.h"
#include"../../Common/Vector3.h"
#include"../../Common/Vector4.h"
#include"../../Common/Quaternion.h"

using namespace physx;
using namespace NCL::Maths;

class PhysXConvert{
public:
	PxVec3 static Vector3ToPxVec3(const Vector3 v3) {
		return PxVec3(v3.x,v3.y,v3.z);
	}

	Vector3 static PxVec3ToVector3(const PxVec3 px3) {
		return	Vector3(px3.x, px3.y, px3.z);
	}

	PxVec4 static Vector4ToPxVec4(const Vector4 v4) {
		return PxVec4(v4.x, v4.y, v4.z,v4.w);
	}

	Vector4 static PxVec4ToVector4(const PxVec4 px4) {
		return	Vector4(px4.x, px4.y, px4.z,px4.w);
	}

	Quaternion static PxQuatToQuat(const PxQuat pq) {
		return Quaternion(pq.x, pq.y, pq.z, pq.w);
	}

	PxQuat static QuatToPxQuat(const Quaternion q) {
		return PxQuat(q.x, q.y, q.z, q.w);
	}
};
