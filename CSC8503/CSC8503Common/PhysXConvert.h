#pragma once
#include"../../include/PhysX/foundation/PxVec3.h"
#include"../../include/PhysX/foundation/PxVec4.h"
#include"../../include/PhysX/foundation/PxQuat.h"
#include"../../Common/Vector3.h"
#include"../../Common/Vector4.h"
#include"../../Common/Quaternion.h"

using namespace physx;

class PhysXConvert{
public:
	static PxVec3 Vector3ToPxVec3(const NCL::Maths::Vector3 v3) {
		return PxVec3(v3.x,v3.y,v3.z);
	}

	static NCL::Maths::Vector3 PxVec3ToVector3(const PxVec3 px3) {
		return	NCL::Maths::Vector3(px3.x, px3.y, px3.z);
	}

	static PxVec4 Vector4ToPxVec4(const NCL::Maths::Vector4 v4) {
		return PxVec4(v4.x, v4.y, v4.z,v4.w);
	}

	static NCL::Maths::Vector4 PxVec4ToVector4(const PxVec4 px4) {
		return	NCL::Maths::Vector4(px4.x, px4.y, px4.z,px4.w);
	}

	static Quaternion PxQuatToQuat(const PxQuat pq) {
		return Quaternion(pq.x, pq.y, pq.z, pq.w);
	}

	static PxQuat QuatToPxQuat(const Quaternion q) {
		return PxQuat(q.x, q.y, q.z, q.w);
	}
};
