#pragma once
#include "GameState.h"
#include "GameTechRenderer.h"
#include "OGLTexture.h"
#include "YiEventSystem.h"
#include "../CSC8503Common/PhysicsXSystem.h"
#include "../../AudioManager/AudioManager.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../../Gameplay/Player.h"
#include "../../Gameplay/Creeper.h"
//#define DEBUG
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

			int GetTeamKill();
			void SetSingleMode();
			void SetMultiMode();
			float tLeft = 120;

			int team1Kill = 1;
			int team2Kill = 0;

		protected:
			void InitialiseAssets();
			void InitialiseUI();
			void InitAbilityContainer();
			Player* InitPlayer(Vector3 pos, GameObjectType team);
			Creeper* InitCreeper(Vector3 pos);
			void AddPaint(GameObject* object, GameObject *collisionSurface, Vector4 color);
		
			void InitWorld();

			void InitDefaultFloor();
			void RegisterEventHandles();
			void HUDUpdate(float dt);

			void CalculateFrameRate(float dt);

			GameTechRenderer*	renderer;
			PhysicsXSystem*		physicsX;
			GameWorld*			world;
			Player*				localPlayer;
			AbilityContainer*	abilityContainer;
			YiEventSystem* eventSystem;

			float FPS = 0.0f;
			float framesPerSecond = 0.0f;
			float lastTime = 0.0f;
			float previousSecond;
			float currentSecond;


			GameUI* gameUI;
			static void _openFirHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _paint(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _GrenadeHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _deleteHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _HitHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _respawnHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _colorzoneHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _damageRangeHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			void UpdateGameObjects(float dt);
		};
	}
}
