#pragma once

#include "../CSC8503Common/PhysicsXSystem.h"
#include "../../AudioManager/AudioManager.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "GameState.h"
#include "YiEventSystem.h"
#include "../../Gameplay/Player.h"
#include "GameUI.h"
#define DEBUG
namespace NCL {
	namespace Rendering
	{
		class RendererBase;
	}
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
			float tLeft = 900;
		protected:
			void InitialiseAssets();
			void InitialiseUI();
			void InitAbilityContainer();
			void InitPlayer(Vector3 pos, GameObjectType team, bool islocal=false);
		
			void InitWorld();

			void InitDefaultFloor();
			void RegisterEventHandles();
			void HUDUpdate(float dt);

			void CalculateFrameRate(float dt);

			RendererBase*	renderer;
			PhysicsXSystem*		physicsX;
			GameWorld*			world;
			Player*				player;
			AbilityContainer*	abilityContainer;
			YiEventSystem* eventSystem;

			float FPS = 0.0f;
			float framesPerSecond = 0.0f;
			float lastTime = 0.0f;
			float previousSecond;
			float currentSecond;

			GameUI* gameUI;
			static void _openFirHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			static void _GrenadeHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			static void _deleteHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			static void _HitHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			static void _respawnHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			static void _colorzoneHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			static void _damageRangeHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
			void UpdateGameObjects(float dt);
		};
	}
}
