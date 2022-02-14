#pragma once
#include "TutorialGame.h"
#include "../CSC8503Common/NetworkObject.h"

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

			GameObject* SpawnPlayer(Vector3 position);

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
			/// <summary>
			/// 
			/// </summary>
			/// <param name="strOutputString"></param>
			/// <param name=""></param>
			/// <returns></returns>
			void ToggleNetworkState(GameObject* object,bool state);
			/// <summary>
			/// 
			/// </summary>
			/// <param name="strOutputString"></param>
			/// <param name=""></param>
			void OutputDebug(const char* strOutputString, ...);

			/// <summary>
			/// Distribute state packages based on client state
			/// </summary>
			/// <param name="o">NetworkObject pointer</param>
			/// <param name="deltaFrame">isDeltaFrame?</param>
			void DistributeSnapshot(NetworkObject* o,bool deltaFrame);

			void UpdateMinimumState();//

			void UpdateStateIDs(ClientPacket* realPacket);


			std::map<int, int> stateIDs;//和上面一个函数一起用

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, GameObject*> serverPlayers;
			GameObject* localPlayer=nullptr;//指定当前客户端的player
			int localPlayerID=-1;//指定当前客户端的识别ID
			int localLastID=-1;//最近接受到的full包的ID
		};
	}
}



