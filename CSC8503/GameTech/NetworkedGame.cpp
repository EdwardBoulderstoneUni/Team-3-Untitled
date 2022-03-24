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
	pc = new PlayerController();
	RegisterHandlers();
}

NetworkedGame::~NetworkedGame() {
	delete thisServer;
	delete thisClient;
}

void NetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);
	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(Event_State, this);

	YiEventSystem::GetMe()->PushEvent(PLAYER_ENTER_WORLD,-1);
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());
	thisClient->RegisterPacketHandler(Sync_Obj, this);
	thisClient->RegisterPacketHandler(Sync_State, this);
	thisClient->RegisterPacketHandler(Event_State, this);
	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Shutdown, this);

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
		timeToNextPacket += 1.0f / 60.0f; //20hz server/client update
	}
	if (thisServer && timeToUpdateMiniState < 0) {
		UpdateMinimumState();
		timeToUpdateMiniState += 30.0f;
	}

	if (!thisServer && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		StartAsServer();
	}
	if (!thisClient && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		//StartAsClient(10,70,33,127);
		StartAsClient(127, 0, 0, 1);
	}
	if (!thisServer && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F11)) {
		EventPacket newPacket;
		newPacket.eventID = PLAYER_EXIT_WORLD;
		newPacket.playerID = localPlayerID;
		thisClient->SendPacket(newPacket);
	}
	if (!thisClient && Window::GetKeyboard()->KeyPressed(KeyboardKeys::F11)) {
		thisServer->Shutdown();
		YiEventSystem::GetMe()->PushEvent(SERVER_SHUT_DOWN);
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
#define INIT \
Vector3 origin = localPlayer->GetTransform().GetPosition();  \
DirectionVec dir = localPlayer->GetDirectionVec();           \
PlayerPro* pro = localPlayer->GetPlayerPro();                \

	if (not localPlayer) return;
	ClientPacket newPacket;
	pc->update(dt);
	Input lastinput = pc->get_inputs();
	if (Window::GetMouse()->ButtonPressed(MouseButtons::LEFT)) {
		newPacket.buttonstates[7] = 1;
	}
	if (lastinput.buttons[jump]) {
		newPacket.buttonstates[0] = 1;
	}
	if (lastinput.buttons[dash]) {
		newPacket.buttonstates[1] = 1;
	}
	if (lastinput.buttons[reload]) {
		newPacket.buttonstates[2] = 1;
	}
	if (lastinput.movement_direction==Vector2(0,1)) {
		newPacket.buttonstates[3] = 1;
	}
	if (lastinput.movement_direction == Vector2(0, -1)) {
		newPacket.buttonstates[4] = 1;
	}
	if (lastinput.movement_direction == Vector2(-1, 0)) {
		newPacket.buttonstates[5] = 1;
	}
	if (lastinput.movement_direction == Vector2(1, 0)) {
		newPacket.buttonstates[6] = 1;
	}
	
	newPacket.angles[0] = lastinput.look_direction.x;
	newPacket.angles[1] = lastinput.look_direction.y;
	newPacket.angles[2] = 0;

	newPacket.lastID = localLastID;
	
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

void NetworkedGame::StartLevel() {

}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	//server
	if (type == Received_State) {
		ClientPacket* realPacket = (ClientPacket*)payload;
		UpdateStateIDs(realPacket);
	
		Player* player =(Player*)networkplayers.find(realPacket->playerID)->second;
		
		Input input = Input();
		input.buttons[jump] = realPacket->buttonstates[0];
		input.buttons[dash] = realPacket->buttonstates[1];
		input.buttons[reload] = realPacket->buttonstates[2];
		input.buttons[attack] = realPacket->buttonstates[7];

		if (realPacket->buttonstates[3] == 1) 
			input.movement_direction = Vector2(0, 1);
		if (realPacket->buttonstates[4] == 1)
			input.movement_direction = Vector2(0, -1);
		if (realPacket->buttonstates[5] == 1)
			input.movement_direction = Vector2(-1, 0);
		if (realPacket->buttonstates[6] == 1)
			input.movement_direction = Vector2(1, 0);
		input.look_direction.x = realPacket->angles[0];
		input.look_direction.y = realPacket->angles[1];
		player->SetLastInput(input);
		player->networkInput = true;
		
		SyncStatePacket newPacket;
		newPacket.state = *player->GetPlayerPro();
		thisServer->SendPacketToPeer(newPacket,realPacket->playerID);
	}
	else if (type == Event_State) {
		EventPacket* realPacket = (EventPacket*)payload;
		switch (realPacket->eventID)
		{
		case PLAYER_ENTER_WORLD:
			YiEventSystem::GetMe()->PushEvent(PLAYER_ENTER_WORLD,realPacket->networkID);
			break;
		case GAME_WORLD_SYN:
			YiEventSystem::GetMe()->PushEvent(GAME_WORLD_SYN,realPacket->networkID);
			break;
		case PLAYER_EXIT_WORLD:
			YiEventSystem::GetMe()->PushEvent(PLAYER_EXIT_WORLD, realPacket->playerID,realPacket->networkID);
			break;
		case PLAYER_OPEN_FIRE:
			for (auto i : networkObjects) {
				if (i->GetNetworkID() == realPacket->networkID) {
					YiEventSystem::GetMe()->PushEvent(PLAYER_OPEN_FIRE, i->GetWorldID());
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	//client
	else if (type == Delta_State) {
		DeltaPacket* realPacket = (DeltaPacket*)payload;
		if (realPacket->objectID >= networkObjects.size())return;
		networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
	}
	else if (type == Full_State) {
		FullPacket* realPacket = (FullPacket*)payload;
		if(realPacket->objectID>=networkObjects.size())return;
		networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
		localLastID = networkObjects[realPacket->objectID]->GetLatestNetworkState().stateID;
	}
	else if (type == Sync_Obj) {
		SyncPacket* realPacket = (SyncPacket*)payload;
		NetworkState state =realPacket->fullState;
		if (realPacket->objType == GameObjectType_team1 
			or realPacket->objType == GameObjectType_team2) {
			Player* newPlayer = nullptr;
			if (realPacket->objType == GameObjectType_team1) 
				newPlayer = InitPlayer(Vector3(20, 50, 0), GameObjectType_team1);
			else
				newPlayer = InitPlayer(Vector3(20, 50, 0), GameObjectType_team2);
			newPlayer->RemoveComponetCamera();
			newPlayer->RemoveComponetInput();
			newPlayer->RemoveComponetPhysics();
			ToggleNetworkState(newPlayer, true);
			newPlayer->GetTransform().SetPosition(state.position);
			newPlayer->GetTransform().SetOrientation(state.orientation);
			if (networkObjects.size() == realPacket->SyncTotalCount) {
				EventPacket newPacket;
				newPacket.eventID = PLAYER_ENTER_WORLD;
				newPacket.networkID = localPlayerID;
				thisClient->SendPacket(newPacket);
			}
		}

	}
	else if (type == Sync_State) {
		SyncStatePacket* realPacket=(SyncStatePacket*)payload;
		*localPlayer->GetPlayerPro() = realPacket->state;
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

		if (networkObjects.size()==0) {
			localPlayerID = realPacket->playerID;
			EventPacket newPacket;
			newPacket.eventID = GAME_WORLD_SYN;
			newPacket.networkID = localPlayerID;
			thisClient->SendPacket(newPacket);
		}
		else {
			YiEventSystem::GetMe()->PushEvent(PLAYER_ENTER_WORLD, realPacket->playerID);
		}

		
	}
	else if (type == Player_Disconnected) {
		PlayerDisconnectPacket* realPacket = (PlayerDisconnectPacket*)payload;
		std::cout << "Client: Player Disconnected!" << std::endl; 
	}
	else if (type == Shutdown) {
		std::cout << "Server shutdown!" << std::endl;
		YiEventSystem::GetMe()->PushEvent(SERVER_SHUT_DOWN);
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
	return it != stateIDs.end() ? it->second : 0;
}

void NetworkedGame::DistributeSnapshot(NetworkObject* o, bool deltaFrame)
{
	for (int i = 0; i < stateIDs.size(); i++)
	{
		GamePacket* newPacket = nullptr;
		int playerState = GetClientStateID(i);
	
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendPacketToPeer(*newPacket, i);
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

void NetworkedGame::RegisterHandlers()
{
	eventSystem->RegisterEventHandle("ENTER_WORLD", _enterHandle,(DWORD64)this);
	eventSystem->RegisterEventHandle("WORLD_SYN", _worldsyncHandle, (DWORD64)this);
	eventSystem->RegisterEventHandle("EXIT_WORLD", _exitHandle, (DWORD64)this);
	eventSystem->RegisterEventHandle("SHUT_DOWN", _serverShutdownHandle, (DWORD64)this);
	eventSystem->RegisterEventHandle("OPEN_FIRE", _openFirHandle, (DWORD64)this);
}

void NetworkedGame::_enterHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	auto game = (NetworkedGame*)dwOwnerData;
	int playerID = stoi(pEvent->vArg[0]);
	if (game->localPlayer) {
		Player* newPlayer = playerID % 2 != 0 ? game->InitPlayer(Vector3(-200, 50, 0), GameObjectType_team1)
			: game->InitPlayer(Vector3(-200, 50, 0), GameObjectType_team2);
		newPlayer->RemoveComponetInput();
		game->ToggleNetworkState(newPlayer, true);

		if (game->thisClient) {
			newPlayer->RemoveComponetPhysics();
		}
		if (game->thisServer) {
			game->networkplayers.insert(std::pair<int, GameObject*>(playerID, newPlayer));
			EventPacket newPacket;
			newPacket.eventID = PLAYER_ENTER_WORLD;
			newPacket.networkID = playerID;
			game->thisServer->SendPacketToPeer(newPacket,playerID);
		}
	}
	else {
		Player* newPlayer = playerID % 2 != 0 ? game->InitPlayer(Vector3(-200, 50, 0), GameObjectType_team1)
			: game->InitPlayer(Vector3(-200, 50, 0), GameObjectType_team2);
		game->localPlayer = newPlayer;
		game->ToggleNetworkState(newPlayer, true);
		game->world->SetMainCamera(newPlayer->GetComponentCamera()->camera);
		if (game->thisClient) {
			newPlayer->RemoveComponetInput();
			newPlayer->RemoveComponetPhysics();
		}
		if (game->thisServer) {
		}
		
	}
}
void NetworkedGame::_worldsyncHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	auto game = (NetworkedGame*)dwOwnerData;
	int playerID = stoi(pEvent->vArg[0]);
	if (game->thisClient) {
		
	}
	if (game->thisServer) {
		for (auto i : game->networkObjects) {
			SyncPacket newPacket;
			switch (i->GetObjType())
			{
			case GameObjectType_team1:
				newPacket.objType = GameObjectType_team1;
				break;
			default:
				break;
			}
			newPacket.SyncTotalCount = game->networkObjects.size();
			newPacket.objectID = i->GetNetworkID();
			newPacket.fullState = i->GetLatestNetworkState();
			game->thisServer->SendPacketToPeer(newPacket, playerID);
		}
	}
}
void NetworkedGame::_exitHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	auto game = (NetworkedGame*)dwOwnerData;
	int playerID = stoi(pEvent->vArg[0]);
	int netID = stoi(pEvent->vArg[1]);
	if (game->thisClient) {
		if (playerID == game->localPlayerID) {
			for (auto i : game->networkObjects) {
				if (game->localPlayer->GetWorldID() == i->GetWorldID())continue;
				YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, i->GetWorldID());
			}
			game->thisClient->Disconnect();
		}
		else {
			for (auto i : game->networkObjects) {
				if (i->GetNetworkID() == netID)
				YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, i->GetWorldID());
			}
		}
	}
	if (game->thisServer) {
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, game->networkplayers.find(playerID)->second->GetWorldID());
		
		EventPacket newPacket;
		newPacket.eventID = PLAYER_EXIT_WORLD;
		for (auto i : game->networkObjects) {
			if (game->networkplayers.find(playerID)->second->GetWorldID() == i->GetWorldID()) {
				newPacket.networkID = i->GetWorldID();
			}
		}
		newPacket.playerID = playerID;
		game->thisServer->SendGlobalPacket(newPacket);
	}
}
void NetworkedGame::_serverShutdownHandle(const EVENT* pEvent, DWORD64 dwOwnerData) {
	auto game = (NetworkedGame*)dwOwnerData;
	for (auto i: game->networkObjects)
	{
		if (game->localPlayer->GetWorldID() == i->GetWorldID())continue;
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE,i->GetWorldID());
	}
}

void NetworkedGame::_openFirHandle(const EVENT* pEvent, DWORD64 dwOwnerData) {
	auto game = (NetworkedGame*)dwOwnerData;
	int worldID = stoi(pEvent->vArg[0]);
	if (game->thisServer) {
		EventPacket newpacket;
		newpacket.eventID = PLAYER_OPEN_FIRE;
		for (auto i : game->networkObjects) {
			if (i->GetWorldID() == worldID) {
				newpacket.networkID = i->GetNetworkID();
			}
		}
		game->thisServer->SendGlobalPacket(newpacket);
	}
	if (game->thisClient) {
		int i = 0;
	}
}
