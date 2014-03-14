#pragma once

#include "Communicator.h"

#include <thread>
#include <vector>

// Forward declaration
namespace sf { class TcpSocket; class TcpListener; }

#define TCP_PROTOCOL_NAME "tcp"

class TcpCommunicator : public Communicator
{
public:
	TcpCommunicator();
	~TcpCommunicator();
	virtual bool Bind(CommunicatorPort_t port);
	virtual void Unbind();
	virtual bool Receive();
	virtual void Connect(CommunicatorHostAddress_t host);
	virtual CommunicatorPort_t GetBoundPort();
	virtual void SendPacket(CommunicatorUpdate_t update);
	virtual CommunicatorUpdate_t GetUpdate();
private:
	std::vector<CommunicatorUpdate_t> m_oUpdateBuffer;
	sf::TcpListener m_oTcpListener;

	static void ReceiveThread(sf::TcpSocket *pTcpSocket,
		std::vector<CommunicatorUpdate_t> *pUpdateBuffer,
		std::mutex *pReceivingThreadMutex);

	std::mutex m_oReceivingThreadMutex;

	static void TcpCommunicator::ListenThread(sf::TcpListener *pListener,
		std::vector<CommunicatorUpdate_t> *pUpdateBuffer,
		std::mutex *pReceivingThreadMutex,
		std::vector<sf::TcpSocket*> *pClientSockets,
		std::mutex *pListeningThreadMutex);

	std::vector<sf::TcpSocket*> m_oClientSockets;

	std::mutex m_oListeningThreadMutex;

	sf::TcpSocket *m_pListeningTcpSocket = nullptr;
};