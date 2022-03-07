#pragma once
#include "ComponetGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class Floor : public ComponetGameObject
		{
		public:
			Floor();
			~Floor();
			void SetUp() override;
		};
	}
}


