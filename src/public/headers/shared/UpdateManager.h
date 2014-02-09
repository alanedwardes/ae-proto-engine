#pragma once

#include "Communicator.h"

enum ClientNetworkUpdateType_t
{
	CLIENT_UPDATE_NONE = 0,
	CLIENT_UPDATE_FULL,
	CLIENT_UPDATE_INITIAL,
	CLIENT_UPDATE_FINAL
};

enum ServerNetworkUpdateType_t
{
	SERVER_UPDATE_NONE = 0,
	SERVER_UPDATE_INITIAL,
	SERVER_UPDATE_FULL,
	SERVER_UPDATE_DELTA,
	SERVER_UPDATE_FINAL
};

class UpdateManager
{
public:
	virtual void ReceiveUpdates() = 0;
	virtual void SendUpdates() = 0;
	virtual void ProcessUpdate(CommunicatorUpdate_t update) = 0;
protected:
	Communicator *m_pCommunicator;
};