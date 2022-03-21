#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"
#include "../../Common/TextureLoader.h"
#define COLLISION_MSG 30

struct MessagePacket : public GamePacket {
	short playerID = -1;
	short messageID = -1;

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
	timeToUpdateMiniState = 0.0f;
	packetsToSnapshot = 0;

	InitialiseAssets();
}

NetworkedGame::~NetworkedGame() {
	delete thisServer;
	delete thisClient;
}

void NetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);

	thisServer->RegisterPacketHandler(Received_State, this);
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
	thisClient->RegisterPacketHandler(Shutdown, this);

	StartLevel();
}

void NetworkedGame::UpdateGame(float dt) {
	timeToNextPacket -= dt;
	timeToUpdateMiniState -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 20.0f; //20hz server/client update
	}
	if (thisServer && timeToUpdateMiniState < 0) {
		UpdateMinimumState();
		timeToUpdateMiniState += 10.0f;
	}

	if (!thisServer && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		StartAsServer();
	}
	if (!thisClient && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		StartAsClient(127, 0, 0, 1);
	}
	if (!thisServer && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F11)) {
		thisClient->Disconnect();
	}
	if (!thisClient && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F11)) {
		thisServer->Shutdown();
	}
	if (localPlayer) {
		inSelectionMode = true;
		UpdatePlayer(dt);
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

	if (Window::GetInterface()->button_down(sprint)) {
		newPacket.buttonstates[0] = 1;  //TODO:fire!
	}
	if (Window::GetInterface()->get_movement().y > 0){
		newPacket.buttonstates[1] = 1;
	}
	if (Window::GetInterface()->get_movement().y < 0) {
		newPacket.buttonstates[2] = 1;
	}
	if (Window::GetInterface()->get_movement().x > 0) {
		newPacket.buttonstates[3] = 1;
	}
	if (Window::GetInterface()->get_movement().x < 0) {
		newPacket.buttonstates[4] = 1;
	}
	newPacket.angles[0] = Window::GetInterface()->get_look_direction().x;
	if (localLastID == -1) return;
	newPacket.lastID = localLastID;
	OutputDebug("localastID: %d", localLastID);
	newPacket.type = Received_State;
	newPacket.playerID = localPlayerID;
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
		DistributeSnapshot(o, deltaFrame);
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

GameObject* NetworkedGame::SpawnPlayer(Vector3 position) {

	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);


	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));

	world->AddGameObject(character);

	return character;
}

void NetworkedGame::StartLevel() {

}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	//server
	if (type == Received_State) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		UpdateStateIDs(realPacket);
		GameObject* player = serverPlayers.find(realPacket->playerID)->second;
		if (!player)return;
		MovePlayerAndFire(player, realPacket->buttonstates, realPacket->angles);
	}
	else if (type == Spawn_Player) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		OutputDebug("[Spawn_Player] call from [PlayerID: %d]", realPacket->playerID);
		GameObject* newPlayer = SpawnPlayer(Vector3(0, 5, 0));
		ToggleNetworkState(newPlayer, true);
		serverPlayers.insert(std::pair<int, GameObject*>(realPacket->playerID, newPlayer));
		for (int i = 0; i < networkObjects.size() - 1; ++i) {
			SpawnPacket* newPacket = nullptr;
			networkObjects[i]->WriteSpawnPacket(&newPacket, i, realPacket->playerID);
			thisServer->SendPacketToPeer(*newPacket, source);
		}
		SpawnPacket* newPacket = nullptr;
		networkObjects[networkObjects.size() - 1]
			->WriteSpawnPacket(&newPacket, int(networkObjects.size() - 1), realPacket->playerID);
		thisServer->SendGlobalPacket(*newPacket);
	}

	//client
	else if (type == Delta_State) {
		DeltaPacket* realPacket = (DeltaPacket*)payload;
		if (realPacket->objectID >= (int)networkObjects.size())return;
		networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
	}
	else if (type == Full_State) {
		FullPacket* realPacket = (FullPacket*)payload;
		if (realPacket->objectID >= (int)networkObjects.size())return;
		if (!networkObjects[realPacket->objectID]->ReadPacket(*realPacket))return;
		localLastID = networkObjects[realPacket->objectID]->GetLatestNetworkState().stateID;
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
		if (localPlayerID != -1)return;
		localPlayerID = realPacket->playerID;
		ClientPacket newPacket;
		newPacket.type = Spawn_Player;
		newPacket.playerID = localPlayerID;
		thisClient->SendPacket(newPacket);
	}
	else if (type == Player_Disconnected) {
		PlayerDisconnectPacket* realPacket = (PlayerDisconnectPacket*)payload;
		std::cout << "Client: Player Disconnected!" << std::endl; //TODO:delete player when client is disconnected
	}
	else if (type == Spawn_Object) {
		SpawnPacket* realPacket = (SpawnPacket*)payload;
		if (realPacket->objectType == ObjectType::Player) {
			GameObject* newPlayer = SpawnPlayer(realPacket->fullState.position);
			if (localPlayerID == realPacket->playerID)
				localPlayer = newPlayer;
			ToggleNetworkState(newPlayer, true);
			if (localLastID == -1)localLastID = realPacket->fullState.stateID;//TODO:Strategy Pattern/Consortium
			OutputDebug("[Spawn_Object] [localLastID:%d]", localLastID);
		}
	}
	else if (type == Shutdown) {
		std::cout << "Server shutdown!" << std::endl;
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

int NetworkedGame::GetClientStateID(int clientID)
{
	std::map<int, int>::iterator it = stateIDs.find(clientID);
	return it != stateIDs.end() ? it->second : -1;
}

void NetworkedGame::DistributeSnapshot(NetworkObject* o, bool deltaFrame)
{
	for (int i = 0; i < stateIDs.size(); i++)
	{
		GamePacket* newPacket = nullptr;
		int playerState = GetClientStateID(i);
		if (playerState != -1) {
			if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
				thisServer->SendPacketToPeer(*newPacket, i);
			}
		}
		delete newPacket;
	}
}

void NetworkedGame::OutputDebug(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf(strBuffer, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(strBuffer);
}

void NetworkedGame::ToggleNetworkState(GameObject* object, bool state)
{
	if (state) {
		object->SetNetworkObject(int(networkObjects.size()));
		networkObjects.emplace_back(object->GetNetworkObject());
	}
	else {
		int networkID = object->GetNetworkObject()->GetNetworkID();
		networkObjects.erase(networkObjects.begin() + networkID);
		object->SetNetworkObject(nullptr);
	}
}

void NetworkedGame::UpdateStateIDs(ClientPacket* realPacket) {
	if (stateIDs.size() == 0)
		stateIDs.insert(std::pair<int, int>(realPacket->playerID, realPacket->lastID));
	else {
		std::map<int, int>::iterator it = stateIDs.find(realPacket->playerID);
		if (it != stateIDs.end()) {
			if (it->second < realPacket->lastID) {
				it->second = realPacket->lastID;
				localLastID = realPacket->lastID;
			}
		}
		else {
			stateIDs.insert(std::pair<int, int>(realPacket->playerID, localLastID));
		}
	}
}

bool NetworkedGame::MovePlayerAndFire(GameObject* player, char buttonstates[8], int angle[3])
{
	int Speed = 2;
	Vector3 position = player->GetTransform().GetPosition();
	Quaternion orientation = player->GetTransform().GetOrientation();
	Vector3 euler = orientation.ToEuler(); //roll yaw pitch
	euler.y -= (angle[0]);
	if (euler.y < 0) {
		euler.y += 360.0f;
	}
	if (euler.y > 360.0f) {
		euler.y -= 360.0f;
	}
	if (buttonstates[0] == 1) {
	}
	if (buttonstates[1] == 1) {
		position += Matrix4::Rotation(euler.y, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * Speed;
	}
	if (buttonstates[2] == 1) {
		position -= Matrix4::Rotation(euler.y, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * Speed;
	}
	if (buttonstates[3] == 1) {
		position += Matrix4::Rotation(euler.y, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * Speed;
	}
	if (buttonstates[4] == 1) {
		position -= Matrix4::Rotation(euler.y, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * Speed;
	}
	player->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(euler.z, euler.y, euler.x));
	player->GetTransform().SetPosition(position);
	return true;
}

void NetworkedGame::UpdatePlayer(float dt)
{
	Vector3 position = localPlayer->GetTransform().GetPosition();

	Quaternion orientation = localPlayer->GetTransform().GetOrientation();
	Vector3 euler = orientation.ToEuler(); //roll yaw pitch

	Vector3 cameraPosition = position + Matrix4::Rotation(localPlayer->GetTransform().GetOrientation().ToEuler().y,
		Vector3(0, 1, 0)) * Vector3(0, 20, 30);

	float yaw = euler.y;
	world->GetMainCamera()->SetPosition(cameraPosition);
	world->GetMainCamera()->SetPitch(-30.0f);
	world->GetMainCamera()->SetYaw(yaw);
}

void NetworkedGame::InitialiseAssets()
{
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};
	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Male1.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}