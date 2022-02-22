#pragma once
#include <thread>
#include <atomic>

#include "NetworkBase.h"

namespace NCL {
	namespace CSC8503 {
		class GameWorld;
		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients);
			~GameServer();

			bool Initialise();
			void Shutdown();

			void SetGameWorld(GameWorld& g);

			void ThreadedUpdate();

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);
			bool SendPacketToPeer(GamePacket& packet, int source);
			ENetPeer* GetClientPeerPointer(int source);

			virtual void UpdateServer();

		protected:
			int			port;
			int			clientMax;
			int			clientCount;
			GameWorld* gameWorld;

			std::atomic<bool> threadAlive;

			std::map<int, ENetPeer*> clientPeerPointers;


			std::thread updateThread;

			int incomingDataRate;
			int outgoingDataRate;
		};
	}
}