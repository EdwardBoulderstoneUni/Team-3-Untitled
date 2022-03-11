#pragma once
#include "ComponentGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Sphere : public ComponentGameObject
		{
		public:
			Sphere();
			~Sphere();
			void SetUp() override;
		};
	}
}

