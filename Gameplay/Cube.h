#pragma once
#include "ComponentGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Cube : public ComponentGameObject
		{
		public:
			Cube();
			~Cube();
			void SetUp() override;
		};
	}
}

