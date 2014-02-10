#pragma once

#include <vector>

#include "shared\UpdateManager.h"

class BasePlayerEntity;

struct UpdateClient_t
{
	UpdateClient_t() : lastServerUpdate(SERVER_UPDATE_NONE),
		lastClientUpdate(CLIENT_UPDATE_NONE) { }

	ServerNetworkUpdateType_t lastServerUpdate;
	ClientNetworkUpdateType_t lastClientUpdate;

	CommunicatorHostAddress_t host;

	long lastReceivedUpdateTime;

	BasePlayerEntity *entity;

	int updateClientId;

    inline bool operator!()
	{
        return (!host && lastServerUpdate == SERVER_UPDATE_NONE &&
			lastClientUpdate == CLIENT_UPDATE_NONE);
    }
};

class ServerUpdateManager : public UpdateManager
{
public:
	ServerUpdateManager(int iListenPort);
	~ServerUpdateManager();
	virtual int GenerateUpdateClientId();
	virtual void ReceiveUpdates();
	virtual void SendUpdates();
	virtual long GetCurrentTime();
	virtual void ProcessUpdate(CommunicatorUpdate_t update);
private:
	virtual void KickClient(UpdateClient_t *pUpdateClient);

	virtual UpdateClient_t* GetUpdateClientByHostAddress(CommunicatorHostAddress_t hostAddress);

	virtual void ProcessInitialClientUpdate(CommunicatorUpdate_t update);
	virtual void ProcessClientUpdate(CommunicatorUpdate_t update);

	virtual void SendInitialClientUpdate(UpdateClient_t *updateClient);
	virtual void SendClientUpdate(UpdateClient_t *updateClient);
	std::vector<UpdateClient_t*> m_oUpdateClients;
	int m_iLastUpdateClientId;
	long m_lStartTime;
};