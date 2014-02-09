#pragma once

#include "shared\UpdateManager.h"

class ClientUpdateManager : public UpdateManager
{
public:
	ClientUpdateManager();
	~ClientUpdateManager();
	virtual void SetConnection(std::string szAddress, int iPort);
	virtual void ReceiveUpdates();
	virtual void SendUpdates();
	virtual void ProcessUpdate(CommunicatorUpdate_t update);
	virtual int GetUpdateClientId() { return m_iUpdateClientId; };
private:
	virtual void ProcessInitialServerUpdate(CommunicatorData_t data);
	virtual void ProcessServerUpdate(CommunicatorData_t data);

	CommunicatorHostAddress_t m_oRemoteHost;
	// Required for NAT traversal: listen
	// to the outgoing data port after send
	virtual void BindToOutgoingPort();
	// Keep track of the local port
	CommunicatorPort_t m_oLocalPort;
	// Last received update
	CommunicatorUpdate_t m_oLastReceivedUpdate;
	// Last sent update
	CommunicatorUpdate_t m_oLastSentUpdate;
	// Set by the server
	int m_iUpdateClientId;
};