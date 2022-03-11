#pragma once
#include "ComponetGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Cube : public ComponetGameObject
		{
		public:
			Cube();
			~Cube();
			void SetUp() override;
		};
	}
}

