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
	thisServer->RegisterPacketHandler(None, this);
	StartLevel();
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);

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
		std::cout << "fire button pressed!" << std::endl;
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
	thisClient->SendPacket(newPacket);
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
		////when a player has sent the server an acknowledgement and store the lastID somewhere.
		////A map between player and an int could work/part of a NetworkPlayer struct

		int playerState = 0;
		GamePacket* newPacket = nullptr;
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
	}
}



void NetworkedGame::SpawnPlayer() {

	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(Vector3(0, 5, 0));

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	character->SetNetworkObject(0);
	lockedObject = character;
	world->AddGameObject(character);
	localPlayer = character;
	networkObjects.emplace_back(character->GetNetworkObject());

}

void NetworkedGame::StartLevel() {
	SpawnPlayer();
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == Received_State) {	//Server version of the game receives these from players
		std::cout << "Server: Received Received_State!" << std::endl;
		ClientPacket* realPacket = (ClientPacket*)payload;
	}
	else if (type == None) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		MovePlayer(localPlayer,realPacket->buttonstates);
		//TODO
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
		FullPacket* realPacket = (FullPacket*)payload;
		if (realPacket->objectID < (int)networkObjects.size()) {
			networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
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
	}
	else if (type == Player_Disconnected) {
		PlayerDisconnectPacket* realPacket = (PlayerDisconnectPacket*)payload;
		std::cout << "Client: Player Disconnected!" << std::endl;
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