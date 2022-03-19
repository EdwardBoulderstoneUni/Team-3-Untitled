#pragma once

#include "../../AudioManager/AudioManager.h"
#include "../../Common/Vector3.h"
#include"../../Common/RendererBase.h"

class PhysicsXSystem;
namespace NCL {
	
	namespace CSC8503 {
		class GameObject;
		class GameWorld;
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing,
			                       const NCL::Maths::Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();

			bool SelectObject();
			bool SelectXObject();

			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(const NCL::Maths::Vector3& position);
			GameObject* AddSphereToWorld(const NCL::Maths::Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const NCL::Maths::Vector3& position, NCL::Maths::Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddCapsuleToWorld(const NCL::Maths::Vector3& position, float halfHeight, float radius,
			                              float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld(const NCL::Maths::Vector3& position);
			GameObject* AddEnemyToWorld(const NCL::Maths::Vector3& position);
			GameObject* AddBonusToWorld(const NCL::Maths::Vector3& position);

			RendererBase*	renderer;
			PhysicsXSystem*		physicsX;
			GameWorld*			world;
	
			bool useGravity;
			bool inSelectionMode;

			float forceMagnitude;

			GameObject* selectionObject = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			NCL::Maths::Vector3 lockedOffset = NCL::Maths::Vector3(0, 14, 20);

			void LockCameraToObject(GameObject* o)
			{
				lockedObject = o;
			}
		};
	}
}
