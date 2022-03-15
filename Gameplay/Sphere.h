#pragma once
#include "ComponentGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Sphere : public ComponentGameObject
		{
		public:
			Sphere();
			~Sphere();
			virtual void SetUp() override;
			virtual void Update(float dt)override;
		};
	}
}

