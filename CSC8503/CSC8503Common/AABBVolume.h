#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector3.h"
namespace NCL {
	class AABBVolume : public CollisionVolume
	{
	public:
		AABBVolume(const NCL::Maths::Vector3& halfDims) {
			type		= VolumeType::AABB;
			halfSizes	= halfDims;
		}
		~AABBVolume() {

		}

		NCL::Maths::Vector3 GetHalfDimensions() const {
			return halfSizes;
		}

	protected:
		NCL::Maths::Vector3 halfSizes;
	};
}
