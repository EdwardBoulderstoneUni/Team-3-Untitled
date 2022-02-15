#pragma once
#include "GameObject.h"
#include "NetworkBase.h"
#include "NetworkState.h"
namespace NCL {
	namespace CSC8503 {
		enum class ObjectType
		{
			Player
		};
		struct FullPacket : public GamePacket {
			int		objectID = -1;
			NetworkState fullState;

			FullPacket() {
				type = Full_State;
				size = sizeof(FullPacket) - sizeof(GamePacket);
			}
		};//

		struct DeltaPacket : public GamePacket {
			int		fullID = -1;
			int		objectID = -1;
			char	pos[3] = {0};
			char	orientation[4] = {0};

			DeltaPacket() {
				type = Delta_State;
				size = sizeof(DeltaPacket) - sizeof(GamePacket);
			}
		};

		struct ClientPacket : public GamePacket {
			int		lastID=-1;
			int		playerID=-1;
			char	buttonstates[8] = {0}; //左右上下 跳蹲 射击 

			ClientPacket() {
				size = sizeof(ClientPacket);
			}
		};

		struct SpawnPacket : public GamePacket {
			int networkID=-1;
			int playerID = -1;
			ObjectType  objectType = {};
			NetworkState fullState;
			SpawnPacket() {
				type = Spawn_Object;
				size = sizeof(SpawnPacket);
			}
		};

		class NetworkObject {
		public:
			NetworkObject(GameObject& o, int id);
			virtual ~NetworkObject();

			//Called by clients
			virtual bool ReadPacket(GamePacket& p);
			//Called by servers
			virtual bool WritePacket(GamePacket** p, bool deltaFrame, int stateID);

			virtual bool WriteSpawnPacket(SpawnPacket** p, int networkID, int playerID);

			void UpdateStateHistory(int minID);

			NetworkState& GetLatestNetworkState();

			int GetNetworkID() { return networkID; }
		protected:

			bool GetNetworkState(int frameID, NetworkState& state);

			virtual bool ReadDeltaPacket(DeltaPacket& p);
			virtual bool ReadFullPacket(FullPacket& p);

			virtual bool WriteDeltaPacket(GamePacket** p, int stateID);
			virtual bool WriteFullPacket(GamePacket** p, int stateID);


			GameObject& object;

			NetworkState lastFullState;

			std::vector<NetworkState> stateHistory;

			int deltaErrors;
			int fullErrors;

			int networkID;//服务器和客户端都会对所有需要同步的物体编号 这个编号对服务器和客户端都是一样的
			//如果有两个玩家 玩家1是1000 玩家2是2000 在客户端和服务器两端他们的编号还是一样的
		};
	}
}



