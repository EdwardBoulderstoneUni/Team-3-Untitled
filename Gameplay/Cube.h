#pragma once
#include "ComponentGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Cube : public ComponentGameObject
		{
		public:
			Cube();
			~Cube();
			virtual void SetUp() override;
			virtual void Update(float dt)override;
		};
	}
}

