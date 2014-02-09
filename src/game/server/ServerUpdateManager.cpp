#include "ServerUpdateManager.h"
#include "shared\UdpCommunicator.h"
#include "shared\WorldManager.h"
#include "shared\INetworked.h"
#include "shared\StreamSerialiser.h"

#include "shared\BasePlayerEntity.h"
#include "shared\Key.h"

extern WorldManager g_oWorldManager;

ServerUpdateManager::ServerUpdateManager(int iListenPort)
{
	assert(iListenPort > 0);
	assert(m_pCommunicator == 0);

	m_iLastUpdateClientId = 0;
	m_pCommunicator = new UdpCommunicator();
	m_pCommunicator->Bind(iListenPort);
}

ServerUpdateManager::~ServerUpdateManager()
{
	delete m_pCommunicator;
}

int ServerUpdateManager::GenerateUpdateClientId()
{
	return m_iLastUpdateClientId++;
}

void ServerUpdateManager::ReceiveUpdates()
{
	while (m_pCommunicator->Receive())
	{
		auto oUpdate = m_pCommunicator->GetUpdate();
		ProcessUpdate(oUpdate);
	}
}

void ServerUpdateManager::SendUpdates()
{
	for (auto updateClient : m_oUpdateClients)
	{
		if (updateClient->lastClientUpdate == CLIENT_UPDATE_INITIAL)
		{
			SendInitialClientUpdate(updateClient);
		}
		else
		{
			SendClientUpdate(updateClient);
		}
	}
}

UpdateClient_t* ServerUpdateManager::GetUpdateClientByHostAddress(CommunicatorHostAddress_t hostAddress)
{
	for (auto i = 0; i != m_oUpdateClients.size(); i++)
	{
		if (m_oUpdateClients[i]->host == hostAddress)
		{
			return m_oUpdateClients[i];
		}
	}

	return nullptr;
}

void ServerUpdateManager::SendInitialClientUpdate(UpdateClient_t *updateClient)
{
	// Initial update - contains level name
	CommunicatorUpdate_t update;
	update.host = updateClient->host;
	update.data << SERVER_UPDATE_INITIAL << updateClient->updateClientId << g_oWorldManager.LevelFilename();
	m_pCommunicator->SendPacket(update);

	// Set the last sent update type
	updateClient->lastServerUpdate = SERVER_UPDATE_INITIAL;
}

void ServerUpdateManager::SendClientUpdate(UpdateClient_t *updateClient)
{
	CommunicatorUpdate_t update;
	update.host = updateClient->host;
	auto oEntities = g_oWorldManager.GetEntities();
	auto iEntitiesInPacket = 0;
	for (auto pEntity : oEntities)
	{
		auto pNetworked = dynamic_cast<IServerNetworked*>(pEntity);
		if (pNetworked)
			iEntitiesInPacket++;
	}

	update.data << SERVER_UPDATE_FULL << iEntitiesInPacket;
	for (auto pEntity : oEntities)
	{
		auto pNetworked = dynamic_cast<IServerNetworked*>(pEntity);
		if (!pNetworked)
			continue;

		update.data << pEntity->id;
		update.data << pEntity->typeId;
		update.data << pNetworked->Serialise();
	}
	updateClient->lastServerUpdate = SERVER_UPDATE_FULL;
	m_pCommunicator->SendPacket(update);
}

void ServerUpdateManager::ProcessUpdate(CommunicatorUpdate_t update)
{
	// The Sorting Hat
	CommunicatorData_t data = update.data;

	// Acquire the house from the
	// first integer of the packet
	int updateType;
	data >> updateType;

	switch (updateType)
	{
	case CLIENT_UPDATE_INITIAL:
		// Gryffindor!
		ProcessInitialClientUpdate(update);
		break;
	case CLIENT_UPDATE_FULL:
		// Hufflepuff!
		ProcessClientUpdate(update);
		break;
	default:
		// Slytherin...
		Debug::WarningMessage("Unhandled packet type %", updateType);
	}
}

void ServerUpdateManager::ProcessInitialClientUpdate(CommunicatorUpdate_t update)
{
	//Debug::DebugMessage("Processing initial update from %", update.host.toString());

	auto *pExistingUpdateClient = GetUpdateClientByHostAddress(update.host);
	if (pExistingUpdateClient)
		return;

	auto updateClientId = GenerateUpdateClientId();

	auto pPlayer = (BasePlayerEntity*)g_oWorldManager.CreateEntity("player");
	pPlayer->playerId = updateClientId;

	auto pUpdateClient = new UpdateClient_t();
	pUpdateClient->host = update.host;
	pUpdateClient->lastClientUpdate = CLIENT_UPDATE_INITIAL;
	pUpdateClient->entity = pPlayer;
	pUpdateClient->updateClientId = updateClientId;
	m_oUpdateClients.push_back(pUpdateClient);
}

void ServerUpdateManager::ProcessClientUpdate(CommunicatorUpdate_t update)
{
	//Debug::DebugMessage("Processing regular update from %", update.host.toString());

	auto pUpdateClient = GetUpdateClientByHostAddress(update.host);
	CommunicatorData_t data = update.data;

	if (!pUpdateClient)
	{
		Debug::WarningMessage("Couldn't find client in database.");
		return;
	}

	int updateType = 0;
	data >> updateType;

	short iPressedKeys;
	data >> iPressedKeys;

	pUpdateClient->entity->pressedKeys = iPressedKeys;
	pUpdateClient->lastClientUpdate = CLIENT_UPDATE_FULL;
}