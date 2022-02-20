#pragma once
#include "PxPhysicsAPI.h"
#include <ctype.h>
#include "../CSC8503/GameTech/TutorialGame.h"

namespace physx {
	class PhysXobject {
	public:
		PhysXobject(PxRigidActor* act, PxMaterial* m);
		~PhysXobject();

		PxMaterial* GetMaterial() const {
			return material;
		}

		PxRigidActor* GetActor() const {
			return actor;
		}

	private:
		PxRigidActor* actor;
		PxMaterial* material;
	};
}
