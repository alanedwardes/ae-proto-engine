#include "ClientUpdateManager.h"
#include "shared\UdpCommunicator.h"
#include "shared\WorldManager.h"
#include "shared\INetworked.h"
#include "shared\BaseGameObject.h"
#include "InputManager.h"

extern WorldManager g_oWorldManager;
extern InputManager *g_pInputManager;
extern std::string g_szWindowTitle;

ClientUpdateManager::ClientUpdateManager()
{
	assert(m_pCommunicator == 0);

	// Create a new communicator
	m_pCommunicator.reset(new UdpCommunicator());

	// Set to an invalid port
	m_oLocalPort = -1;
}

void ClientUpdateManager::SetConnection(std::string szAddress, int iPort)
{
	m_oRemoteHost.ipV4Address = CommunicatorIpV4Address_t(szAddress);
	m_oRemoteHost.port = iPort;
}

void ClientUpdateManager::ReceiveUpdates()
{
	while (m_pCommunicator->Receive())
	{
		auto oUpdate = m_pCommunicator->GetUpdate();
		Debug::LogMessage("Processing update from % (% bytes)",
		oUpdate.host.toString(), oUpdate.data.getDataSize());
		ProcessUpdate(oUpdate);
	}
}

void ClientUpdateManager::SendUpdates()
{
	int updateType = 0;
	if (!(m_oLastReceivedUpdate.data >> updateType))
	{
		CommunicatorUpdate_t update;
		update.data << CLIENT_UPDATE_INITIAL;
		update.host = m_oRemoteHost;
		m_oLastSentUpdate = update;
		m_pCommunicator->SendPacket(update);
	}
	else
	{
		CommunicatorUpdate_t update;
		update.data << CLIENT_UPDATE_FULL;
		update.data << g_pInputManager->SampleInput();
		update.data << g_pInputManager->SampleMousePosition();
		update.host = m_oRemoteHost;
		m_oLastSentUpdate = update;
		m_pCommunicator->SendPacket(update);
	}

	// Bind to a port to
	// listen to data returning
	BindToOutgoingPort();
}

void ClientUpdateManager::BindToOutgoingPort()
{
	// If the local port has changed
	// (or isn't 0 anymore, initial update)
	// then listen to it (NAT traversal)
	CommunicatorPort_t oBoundPort = m_pCommunicator->GetBoundPort();
	if (oBoundPort != m_oLocalPort)
	{
		m_pCommunicator->Unbind();
		m_pCommunicator->Bind(oBoundPort);
		m_oLocalPort = oBoundPort;
	}
}

void ClientUpdateManager::ProcessUpdate(CommunicatorUpdate_t update)
{
	m_oLastReceivedUpdate = update;
	CommunicatorData_t data = update.data;

	int updateType;
	data >> updateType;

	switch (updateType)
	{
	case SERVER_UPDATE_INITIAL:
		// Gryffindor!
		ProcessInitialServerUpdate(update.data);
		break;
	case SERVER_UPDATE_FULL:
		// Hufflepuff!
		ProcessServerUpdate(update.data);
		break;
	default:
		// Slytherin...
		Debug::WarningMessage("Unhandled packet type %", updateType);
	}
}

void ClientUpdateManager::ProcessInitialServerUpdate(CommunicatorData_t data)
{
	int updateType;
	std::string szLevelName;
	if (data >> updateType >> m_iUpdateClientId >> szLevelName)
		g_oWorldManager.LoadLevel(szLevelName);
	else
		Debug::WarningMessage("Initial packet corrupted.");
}

void ClientUpdateManager::ProcessServerUpdate(CommunicatorData_t data)
{
	// First field is the update type
	int updateType;
	data >> updateType;
	
	// Second is the number of entities in this packet
	int iEntitiesInPacket;
	data >> iEntitiesInPacket;

	for (int i = 0; i < iEntitiesInPacket; i++)
	{
		// First field is the entity ID
		int oEntityId;
		data >> oEntityId;

		// If the ID is positive
		if (oEntityId >= 0)
		{
			// Second is entity type
			int iEntityType;
			data >> iEntityType;

			// Third is the serialised entity data
			std::string szSerialised;
			data >> szSerialised;

			// Try to find it
			auto pEntity = g_oWorldManager.GetEntityById(oEntityId);
			if (pEntity == nullptr)
			{
				// Else, create the entity using its type
				pEntity = g_oWorldManager.CreateEntity(iEntityType);
			}

			// Assert we've got one
			assert(pEntity);

			// Cast it to a networked entity
			auto pNetworked = dynamic_cast<IClientNetworked*>(pEntity);

			// Assert the entity is actually networked
			assert(pNetworked);

			// Pass the serialised data to the entity to deserialise
			pNetworked->Unserialise(szSerialised);
		}
		else
		{
			// The ID was negative, meaning this entity
			// is marked for deletion
			auto iRemovalId = abs(oEntityId);

			// Remove it, after using abs() to make positive
			auto pEntity = g_oWorldManager.GetEntityById(iRemovalId);
			if (pEntity != nullptr)
			{
				pEntity->deleted = true;
			}
		}
	}
}