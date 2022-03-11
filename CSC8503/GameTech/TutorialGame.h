#pragma once
#include "GameTechRenderer.h"
#include "OGLTexture.h"
#include "../CSC8503Common/PhysicsXSystem.h"
#include "../../AudioManager/AudioManager.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "GameState.h"
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
			virtual void UpdateRender(float dt);

			GameUI* GetUI()const { return gameUI; }
			void StartRender()const { renderer->Render(); }

			void SetSingleMode();
			void SetMultiMode();
		protected:
			void InitialiseAssets();
			void InitialiseUI();
			void InitCamera();
			void UpdateKeys();


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
			void AmmoLeft();
			void TimeLeft(float dt);

			bool SelectObject();
			bool SelectXObject();

			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();
			void CalculateFrameRate(float dt);

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
			bool DebugMode;

			float forceMagnitude;
			float tLeft = 60;

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

			float FPS = 0.0f;
			float framesPerSecond = 0.0f;
			float lastTime = 0.0f;
			float previousSecond;
			float currentSecond;

			void LockCameraToObject(GameObject* o)
			{
				lockedObject = o;
			}

		

			GameUI* gameUI;


		protected:
			static void _openFirHandle(const EVENT* pEvent, UINT dwOwnerData);
			static TutorialGame* p_self;
			static TutorialGame* getMe() { return p_self; }
		};
	}
}
