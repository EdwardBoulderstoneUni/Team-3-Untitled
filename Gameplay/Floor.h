#pragma once
#include "ComponentGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Floor : public ComponentGameObject
		{
		public:
			Floor();
			~Floor();
			void SetUp() override;
		};
	}
}


