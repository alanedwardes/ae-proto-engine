#pragma once

#include "Communicator.h"

#include <thread>
#include <vector>

namespace sf { class UdpSocket; }

class UdpCommunicator : public Communicator
{
public:
	UdpCommunicator();
	~UdpCommunicator();
	virtual bool Bind(CommunicatorPort_t port);
	virtual void Unbind();
	virtual bool Receive();
	virtual CommunicatorPort_t GetBoundPort();
	virtual void SendPacket(CommunicatorUpdate_t update);
	virtual CommunicatorUpdate_t GetUpdate();
private:
	static void ReceiveThread(sf::UdpSocket *pUdpSocket,
		std::vector<CommunicatorUpdate_t> *pUpdateBuffer,
		std::mutex *pReceivingThreadMutex);
	std::vector<CommunicatorUpdate_t> m_oUpdateBuffer;
	std::thread *m_oReceivingThread;
	sf::UdpSocket m_oUdpSocket;
	std::mutex m_oReceivingThreadMutex;
};