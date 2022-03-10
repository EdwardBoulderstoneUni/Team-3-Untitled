#pragma once
#include "GameTechRenderer.h"
#include "OGLTexture.h"
#include "../CSC8503Common/PhysicsXSystem.h"
#include "../../AudioManager/AudioManager.h"
#include "YiEventSystem.h"
#include "../../Gameplay/Player.h"
namespace NCL {
	namespace CSC8503 {
		enum CameraMode {
			LockedObject,
			DebugObject,
			PlayerLock
		};

		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);

		protected:
			void InitialiseAssets();

			void InitAbilityContainer();
			void InitPlayer(Vector3 pos, GameObjectType team);
		
			void InitWorld();

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing,
			                       const Vector3& cubeDims);
			void InitDefaultFloor();
			void RegisterEventHandles();
			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius,
			                              float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddPaint(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

			GameTechRenderer*	renderer;
			PhysicsXSystem*		physicsX;
			GameWorld*			world;
			Player*				player;
			AbilityContainer*	abilityContainer;
			YiEventSystem* eventSystem;
	
			bool inSelectionMode;
			bool camFollowPlayer;

			float forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;

			//Coursework Meshes
			OGLMesh* charMeshA = nullptr;
			OGLMesh* charMeshB = nullptr;
			OGLMesh* enemyMesh = nullptr;
			OGLMesh* bonusMesh = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, 20);

			void LockCameraToObject(GameObject* o)
			{
				lockedObject = o;
			}


		protected:
			static void _testhandle(const EVENT* pEvent, UINT dwOwnerData);
			static TutorialGame* p_self;
			static TutorialGame* getMe() { return p_self; }
		};
	}
}
