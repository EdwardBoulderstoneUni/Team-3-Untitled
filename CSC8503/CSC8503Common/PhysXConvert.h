#pragma once
#include"../../include/PhysX/foundation/PxVec3.h"
#include"../../include/PhysX/foundation/PxVec4.h"
#include"../../include/PhysX/foundation/PxQuat.h"
#include"../../Common/Vector3.h"
#include"../../Common/Vector4.h"
#include"../../Common/Quaternion.h"

using namespace physx;
using namespace NCL::Maths;

class PhysXConvert{
public:
	static PxVec3 Vector3ToPxVec3(const Vector3 v3) {
		return PxVec3(v3.x,v3.y,v3.z);
	}

	static Vector3 PxVec3ToVector3(const PxVec3 px3) {
		return	Vector3(px3.x, px3.y, px3.z);
	}

	static PxVec4 Vector4ToPxVec4(const Vector4 v4) {
		return PxVec4(v4.x, v4.y, v4.z,v4.w);
	}

	static Vector4 PxVec4ToVector4(const PxVec4 px4) {
		return	Vector4(px4.x, px4.y, px4.z,px4.w);
	}

	static Quaternion PxQuatToQuat(const PxQuat pq) {
		return Quaternion(pq.x, pq.y, pq.z, pq.w);
	}

	static PxQuat QuatToPxQuat(const Quaternion q) {
		return PxQuat(q.x, q.y, q.z, q.w);
	}
	static PxTransform TransformToPxTransform(const Transform trans) {
		PxTransform px = PxTransform();
		px.p = Vector3ToPxVec3(trans.GetPosition());
		px.q = QuatToPxQuat(trans.GetOrientation());
		return px;
	}
};
