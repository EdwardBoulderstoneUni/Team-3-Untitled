#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector3.h"

namespace NCL
{
	class OBBVolume : CollisionVolume
	{
	public:
		OBBVolume(const Vector3& halfDims)
		{
			type = VolumeType::OBB;
			halfSizes = halfDims;
		}

		~OBBVolume()
		{
		}

		Vector3 GetHalfDimensions() const
		{
			return halfSizes;
		}

	protected:
		Vector3 halfSizes;
	};
}
