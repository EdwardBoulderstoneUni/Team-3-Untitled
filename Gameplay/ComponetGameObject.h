#pragma once
#include "../CSC8503/CSC8503Common/GameObject.h"
#include "ComponentCamera.h"
#include "ComponentInput.h"
#include "ComponentPhysics.h"
namespace NCL {
	namespace CSC8503 {
		class ComponetGameObject : public GameObject {
		public:
			ComponetGameObject();
			~ComponetGameObject();
			ComponentCamera* GetComponentCamera();
			ComponentInput* GetComponentInput();
			ComponentPhysics* GetComponentPhysics();
			virtual void SetUp() = 0;
			void Update(float dt);
			void InitAllComponet();
			void PushComponet(Component* newCom);
		private:
			void UpdateComponents(float dt);
			void InitComponents();
			Component* components[Component::MAX_COM_TYPE];
		};
	}
}