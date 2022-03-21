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
			float tLeft = 10;

		protected:
			void InitialiseAssets();
			void InitialiseUI();
			void InitAbilityContainer();
			Player* InitPlayer(Vector3 pos, GameObjectType team);
		
			void InitWorld();

			void InitDefaultFloor(Vector3 position,Vector4 color);
			void RegisterEventHandles();
			void HUDUpdate(float dt);

			void CalculateFrameRate(float dt);

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius,
			                              float inverseMass = 10.0f);

		
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddPaint(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

			GameTechRenderer*	renderer;
			PhysicsXSystem*		physicsX;
			GameWorld*			world;
			Player*				localPlayer;
			AbilityContainer*	abilityContainer;
			YiEventSystem* eventSystem;
	
			bool DebugMode;
			
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

			float FPS = 0.0f;
			float framesPerSecond = 0.0f;
			float lastTime = 0.0f;
			float previousSecond;
			float currentSecond;

			GameUI* gameUI;
			static void _openFirHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _deleteHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _HitHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _respawnHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _colorzoneHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			void UpdateGameObjects(float dt);
		};
	}
}
