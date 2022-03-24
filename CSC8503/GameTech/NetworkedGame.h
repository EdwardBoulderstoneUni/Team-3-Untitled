#pragma once
#include "TutorialGame.h"
#include "../CSC8503Common/NetworkObject.h"
#include "../../Gameplay/PlayerController.h"
namespace NCL {
	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		class NetworkedGame : public TutorialGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

			/// <summary>
			/// Get the status of the corresponding client
			/// </summary>
			/// <param name="clientID">ID of the client</param>
			/// <returns>Client's state ID/-1</returns>
			int GetClientStateID(int clientID);

			

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
		
			void ToggleNetworkState(GameObject* object, bool state);
		
			void OutputDebug(const char* strOutputString, ...);

			void DistributeSnapshot(NetworkObject* o, bool deltaFrame);

			void UpdateMinimumState();

			void UpdateStateIDs(ClientPacket* realPacket);

			void RegisterHandlers();
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			float timeToUpdateMiniState;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, GameObject*> networkplayers;

			int localLastID =0;
			int localPlayerID;

			PlayerController* pc;

			static void _enterHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _worldsyncHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _exitHandle(const EVENT* pEvent, DWORD64 dwOwnerData);
			static void _serverShutdownHandle(const EVENT* pEvent, DWORD64 dwOwnerData);

			static void _openFirHandle(const EVENT* pEvent, DWORD64 dwOwnerData);

		};
	}
}


