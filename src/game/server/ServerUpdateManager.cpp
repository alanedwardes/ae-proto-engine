#include "ServerUpdateManager.h"
#include "shared\UdpCommunicator.h"
#include "shared\WorldManager.h"
#include "shared\INetworked.h"
#include "shared\StreamSerialiser.h"

#include "shared\BasePlayerEntity.h"
#include "shared\Key.h"

#include <chrono>

extern WorldManager g_oWorldManager;
extern Manifest *g_pSettings;

ServerUpdateManager::ServerUpdateManager(int iListenPort)
{
	assert(iListenPort > 0);
	assert(m_pReceivingCommunicator == nullptr && m_pSendingCommunicator == nullptr);

	m_lStartTime = std::clock();
	m_iLastUpdateClientId = 0;

	m_pSendingCommunicator.reset(new UdpCommunicator());
	m_pReceivingCommunicator.reset(new UdpCommunicator());
	m_pReceivingCommunicator->Bind(iListenPort);
}

int ServerUpdateManager::GenerateUpdateClientId()
{
	return m_iLastUpdateClientId++;
}

void ServerUpdateManager::ReceiveUpdates()
{
	while (m_pReceivingCommunicator->Receive())
	{
		auto oUpdate = m_pReceivingCommunicator->GetUpdate();
		ProcessUpdate(oUpdate);
	}
}

long ServerUpdateManager::GetCurrentTime()
{
	return (std::clock() - m_lStartTime);
}

void ServerUpdateManager::SendUpdates()
{
	long lCurrentTime = GetCurrentTime();
	int iClientTimeout = g_pSettings->GetInt("client_timeout", 10) * 1000;

	for (auto pUpdateClient : m_oUpdateClients)
	{
		if (pUpdateClient == nullptr)
			continue;

		// Kick inactive clients
		if (pUpdateClient->lastReceivedUpdateTime < lCurrentTime - iClientTimeout)
		{
			KickClient(pUpdateClient);
			continue;
		}

		if (pUpdateClient->lastClientUpdate == CLIENT_UPDATE_INITIAL)
		{
			SendInitialClientUpdate(pUpdateClient);
		}
		else
		{
			SendClientUpdate(pUpdateClient);
		}
	}
}

void ServerUpdateManager::KickClient(UpdateClient_t *pUpdateClient)
{
	m_oUpdateClients[pUpdateClient->updateClientId] = nullptr;
	pUpdateClient->entity->deleted = true;
}

UpdateClient_t* ServerUpdateManager::GetUpdateClientByHostAddress(CommunicatorHostAddress_t hostAddress)
{
	for (auto i = 0; i != m_oUpdateClients.size(); i++)
	{
		if (m_oUpdateClients[i] == nullptr)
			continue;

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
	m_pSendingCommunicator->SendPacket(update);

	// Set the last sent update type
	updateClient->lastServerUpdate = SERVER_UPDATE_INITIAL;
}

void ServerUpdateManager::SendClientUpdate(UpdateClient_t *updateClient)
{
	CommunicatorUpdate_t update;
	update.host = updateClient->host;
	auto oEntities = g_oWorldManager.GetGameObjects();
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

		// If the entity is deleted, send the id as negative
		if (pEntity->deleted)
		{
			update.data << -pEntity->id;
		}
		else
		{
			update.data << pEntity->id;
			update.data << pEntity->typeId;
			update.data << pNetworked->Serialise();
		}
	}
	updateClient->lastServerUpdate = SERVER_UPDATE_FULL;
	m_pSendingCommunicator->SendPacket(update);
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
	Debug::DebugMessage("Processing initial update from %", update.host.toString());

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
	pUpdateClient->lastReceivedUpdateTime = GetCurrentTime();
	m_oUpdateClients.push_back(pUpdateClient);
}

void ServerUpdateManager::ProcessClientUpdate(CommunicatorUpdate_t update)
{
	Debug::DebugMessage("Processing regular update from %", update.host.toString());

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
	pUpdateClient->lastReceivedUpdateTime = GetCurrentTime();
}