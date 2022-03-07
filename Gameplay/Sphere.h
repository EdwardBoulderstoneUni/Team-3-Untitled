#pragma once
#include "ComponetGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Sphere : public ComponetGameObject
		{
		public:
			Sphere();
			~Sphere();
			void SetUp() override;
		};
	}
}

