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
			char	pos[3];
			char	orientation[4];

			DeltaPacket() {
				type = Delta_State;
				size = sizeof(DeltaPacket) - sizeof(GamePacket);
			}
		};

		struct ClientPacket : public GamePacket {
			int		lastID;//���߷��������ڵĿͻ�������ʲôһ��stateλ����
			int		playerID;
			char	buttonstates[8]; //�������� ���� ��� 

			ClientPacket() {
				size = sizeof(ClientPacket);
			}
		};

		struct SpawnPacket : public GamePacket {
			int networkID;
			ObjectType  objectType;
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

			virtual bool WriteSpawnPacket(SpawnPacket** p, int networkID);

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

			int networkID;//�������Ϳͻ��˶����������Ҫͬ���������� �����ŶԷ������Ϳͻ��˶���һ����
			//������������ ���1��1000 ���2��2000 �ڿͻ��˺ͷ������������ǵı�Ż���һ����
		};
	}
}



