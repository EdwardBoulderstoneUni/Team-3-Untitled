#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"

#define COLLISION_MSG 30

struct MessagePacket : public GamePacket {
	short playerID;
	short messageID;

	MessagePacket() {
		type = Message;
		size = sizeof(short) * 2;
	}
};

NetworkedGame::NetworkedGame() {
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket = 0.0f;
	packetsToSnapshot = 0;
}

NetworkedGame::~NetworkedGame() {
	delete thisServer;
	delete thisClient;
}

void NetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);

	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(Received_Command, this);
	thisServer->RegisterPacketHandler(Spawn_Player, this);
	StartLevel();
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Spawn_Object, this);

	StartLevel();
}

void NetworkedGame::UpdateGame(float dt) {
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 20.0f; //20hz server/client update
	}

	if (!thisServer && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		StartAsServer();
	}
	if (!thisClient && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		StartAsClient(127, 0, 0, 1);
	}

	TutorialGame::UpdateGame(dt);
}

void NetworkedGame::UpdateAsServer(float dt) {
	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}
}

void NetworkedGame::UpdateAsClient(float dt) {
	ClientPacket newPacket;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SHIFT)) {
		newPacket.buttonstates[0] = 1;
		newPacket.lastID = 0;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		newPacket.buttonstates[1] = 1;
		newPacket.lastID = 0;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		newPacket.buttonstates[2] = 1;
		newPacket.lastID = 0;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		newPacket.buttonstates[3] = 1;
		newPacket.lastID = 0;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		newPacket.buttonstates[4] = 1;
		newPacket.lastID = 0;
	}
	if (localLastID != -1) {
		newPacket.lastID = localLastID;
		//newPacket.type = Received_Command;
		newPacket.type = Received_State;
		newPacket.playerID = localPlayerID;
		thisClient->SendPacket(newPacket);
	}
}

void NetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}

		////TODO - 
		////when a player has sent the server an acknowledgement and store the lastID somewhere.   ----stateIDs
		////A map between player and an int could work/part of a NetworkPlayer struct
		
		int playerState = 0; //导致了闪烁的问题   ！！！！
		GamePacket* newPacket = nullptr; //这里的问题 这里没有改变 导致deleta的full id一直没变
		std::map<int, int>::iterator it;
		it = stateIDs.find(0);
		if(it!=stateIDs.end())
			playerState = stateIDs.find(0)->second;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
}

void NetworkedGame::UpdateMinimumState() {
	//Periodically remove old data from the server
	int minID = INT_MAX;
	int maxID = 0; //we could use this to see if a player is lagging behind?

	for (auto i : stateIDs) {
		minID = min(minID, i.second);
		maxID = max(maxID, i.second);
	}
	//every client has acknowledged reaching at least state minID
	//so we can get rid of any old states!
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
		//清理客户端里面存的快照  服务器端也有快照
	}
}



GameObject* NetworkedGame::SpawnPlayer(Vector3 position) {

	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(character);
	return character;
}

void NetworkedGame::StartLevel() {

}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == Received_State) {	//Server version of the game receives these from players
		ClientPacket* realPacket = (ClientPacket*)payload;

		//先找如果有 更新 没有添加
		std::map<int,int>::iterator it=stateIDs.find(realPacket->playerID);
		if (it != stateIDs.end())
			it->second = realPacket->lastID;
		else
			stateIDs.insert(std::pair<int, int>(realPacket->playerID, realPacket->lastID));

		GameObject* player = serverPlayers.find(realPacket->playerID)->second;
		if (player)
		{
			MovePlayer(player, realPacket->buttonstates);
		}
	}
	/*else if (type == Received_Command) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		GameObject* player = serverPlayers.find(realPacket->playerID)->second;
		if (player)
		{
			MovePlayer(player,realPacket->buttonstates);
		}
	}*/
	else if (type == Spawn_Player) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		std::cout << "Spawn_Player!" << realPacket->playerID <<std::endl;

		GameObject* newPlayer;
		newPlayer = SpawnPlayer(Vector3(0, 5, 0));
		newPlayer->SetNetworkObject(networkObjects.size());
		networkObjects.emplace_back(newPlayer->GetNetworkObject());
		serverPlayers.insert(std::pair<int, GameObject*>(realPacket->playerID,newPlayer));

		for (int i = 0; i < networkObjects.size(); ++i)
		{
			SpawnPacket* newPacket=nullptr;
			networkObjects[i]->WriteSpawnPacket(&newPacket,i);
			thisServer->SendGlobalPacket(*newPacket);
		}
		
	}
	//CLIENT version of the game will receive these from the servers
	else if (type == Delta_State) {
		std::cout << "Client: Received DeltaPacket!" << std::endl;
		DeltaPacket* realPacket = (DeltaPacket*)payload;
		if (realPacket->objectID < (int)networkObjects.size()) {
			networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
		}
	}
	else if (type == Full_State) {
		std::cout << "Client: Received FullPacket!" << std::endl;
		FullPacket* realPacket = (FullPacket*)payload;
		if (realPacket->objectID < (int)networkObjects.size()) {
			networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
			localLastID = realPacket->fullState.stateID;
		}
		
	}
	else if (type == Message) {
		MessagePacket* realPacket = (MessagePacket*)payload;
		if (realPacket->messageID == COLLISION_MSG) {
			std::cout << "Client: Received collision message!" << std::endl;
		}
	}
	else if (type == Player_Connected) {
		NewPlayerPacket* realPacket = (NewPlayerPacket*)payload;
		std::cout << "Client: New player connected!" << std::endl;
		if (localPlayerID == -1) {
			localPlayerID = realPacket->playerID;
			ClientPacket newPacket;
			newPacket.type = Spawn_Player;
			newPacket.playerID = localPlayerID;
			thisClient->SendPacket(newPacket);
		}
	}
	else if (type == Player_Disconnected) {
		PlayerDisconnectPacket* realPacket = (PlayerDisconnectPacket*)payload;
		std::cout << "Client: Player Disconnected!" << std::endl;
	}
	else if (type == Spawn_Object) {
		SpawnPacket* realPacket = (SpawnPacket*)payload;
		
		if (realPacket->objectType == ObjectType::Player) {
			GameObject* newPlayer;
			newPlayer = SpawnPlayer(realPacket->fullState.position);
			newPlayer->SetNetworkObject(realPacket->fullState.stateID);
			networkObjects.emplace_back(newPlayer->GetNetworkObject());
		}

	}
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b) {
	if (thisServer) { //detected a collision between players!
		MessagePacket newPacket;
		newPacket.messageID = COLLISION_MSG;
		newPacket.playerID = a->GetPlayerNum();

		thisClient->SendPacket(newPacket);

		newPacket.playerID = b->GetPlayerNum();
		thisClient->SendPacket(newPacket);
	}
}

