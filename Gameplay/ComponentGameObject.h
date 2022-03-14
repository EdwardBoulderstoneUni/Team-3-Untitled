#pragma once
#include "../CSC8503/CSC8503Common/GameObject.h"
#include "ComponentCamera.h"
#include "ComponentInput.h"
#include "ComponentPhysics.h"
#include "../CSC8503/CSC8503Common/PhysXConvert.h"
namespace NCL {
	namespace CSC8503 {
		class ComponentGameObject : public GameObject {
		public:
			ComponentGameObject();
			~ComponentGameObject();
			ComponentCamera* GetComponentCamera();
			ComponentInput* GetComponentInput();
			ComponentPhysics* GetComponentPhysics();
			virtual void SetUp() = 0;
			virtual void Update(float dt);
			void InitAllComponent();
			void PushComponent(Component* newCom);
		private:
			void UpdateComponents(float dt);
			void InitComponents();
			Component* components[Component::MAX_COM_TYPE];
		};
	}
}