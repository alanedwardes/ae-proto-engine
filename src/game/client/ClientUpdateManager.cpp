#include "ClientUpdateManager.h"
#include "TcpCommunicator.h"
#include "UdpCommunicator.h"
#include "INetworked.h"
#include "BaseGameObject.h"
#include "InputManager.h"
#include "Locator.h"
#include "IWorldManager.h"
#include "GameState.h"

ClientUpdateManager::ClientUpdateManager()
{
	assert(m_pCommunicator == 0);

	// Create a new communicator from the
	// protocol specified in the settings manifest
	auto pSettingsManifest = Locator::GameState()->Settings();
	auto szProtocolName = pSettingsManifest->GetString("connect_protocol");

	if (szProtocolName == TCP_PROTOCOL_NAME)
	{
		m_pCommunicator.reset(new TcpCommunicator());
	}
	else //if (szProtocolName == UDP_PROTOCOL_NAME) - assume UDP
	{
		m_pCommunicator.reset(new UdpCommunicator());
	}

	// Set to an invalid port
	m_oLocalPort = -1;
}

void ClientUpdateManager::SetConnection(std::string szAddress, int iPort)
{
	m_oRemoteHost.ipV4Address = CommunicatorIpV4Address_t(szAddress);
	m_oRemoteHost.port = iPort;

	auto pTcpCommunicator = dynamic_cast<TcpCommunicator*>(m_pCommunicator.get());
	if (pTcpCommunicator != nullptr)
	{
		pTcpCommunicator->Connect(m_oRemoteHost);
	}
}

void ClientUpdateManager::ReceiveUpdates()
{
	while (m_pCommunicator->Receive())
	{
		auto oUpdate = m_pCommunicator->GetUpdate();
		//Debug::LogMessage("Processing update from % (% bytes)",
		//oUpdate.host.toString(), oUpdate.data.getDataSize());
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
		update.data << Locator::InputManager()->SampleInput();
		update.data << Locator::InputManager()->SampleMousePosition();
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
	{
		auto pGameState = (GameState*)Locator::GameState();
		pGameState->SetUpdateClientId(m_iUpdateClientId);
		Locator::WorldManager()->LoadLevel(szLevelName);
	}
	else
	{
		Debug::WarningMessage("Initial packet corrupted.");
	}
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
			auto pEntity = Locator::WorldManager()->GetEntityById(oEntityId);
			if (pEntity == nullptr)
			{
				// Else, create the entity using its type
				pEntity = Locator::WorldManager()->CreateEntity(iEntityType);
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
			auto pEntity = Locator::WorldManager()->GetEntityById(iRemovalId);
			if (pEntity != nullptr)
			{
				pEntity->deleted = true;
			}
		}
	}
}