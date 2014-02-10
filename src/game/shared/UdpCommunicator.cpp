#include "UdpCommunicator.h"

UdpCommunicator::UdpCommunicator()
{
}

UdpCommunicator::~UdpCommunicator()
{
	// Stop listening to the port
	Unbind();
}

bool UdpCommunicator::Bind(CommunicatorPort_t port)
{
	sf::Socket::Status status = m_oUdpSocket.bind(port);

	if (status == sf::Socket::Done)
		Debug::DebugMessage("Socket listening on port %", port);
	else
		Debug::WarningMessage("Error (%) binding to port %", status, port);

	if (status != sf::Socket::Done)
		return false;

	// Start a receiving thread
	m_oReceivingThread = new std::thread(&ReceiveThread, &m_oUdpSocket,
		&m_oUpdateBuffer, &m_oReceivingThreadMutex);

	return true;
}

void UdpCommunicator::Unbind()
{
	m_oUdpSocket.unbind();
}

void UdpCommunicator::SendPacket(CommunicatorUpdate_t update)
{
	m_oUdpSocket.send(update.data, update.host.ipV4Address, update.host.port);
}

void UdpCommunicator::ReceiveThread(sf::UdpSocket *pUdpSocket,
	std::vector<CommunicatorUpdate_t> *pUpdateBuffer, std::mutex *pReceivingThreadMutex)
{
	sf::Socket::Status status;
	do
	{
		CommunicatorIpV4Address_t sender;
		CommunicatorPort_t port;
		CommunicatorData_t data;

		status = pUdpSocket->receive(data, sender, port);

		if (status != sf::Socket::Done)
		{
			Debug::WarningMessage("Socket disconnected (%)", status);
		}
		else
		{
			CommunicatorUpdate_t oUpdate;
			oUpdate.data = data;
			oUpdate.host.ipV4Address = sender;
			oUpdate.host.port = port;

			pReceivingThreadMutex->lock();
			pUpdateBuffer->push_back(oUpdate);
			pReceivingThreadMutex->unlock();
		}
	} while(status == sf::Socket::Done);
}

CommunicatorUpdate_t UdpCommunicator::GetUpdate()
{
	m_oReceivingThreadMutex.lock();
	auto oUpdate = m_oUpdateBuffer.back();
	m_oUpdateBuffer.pop_back();
	m_oReceivingThreadMutex.unlock();
	return oUpdate;
}

bool UdpCommunicator::Receive()
{
	if (!m_oReceivingThreadMutex.try_lock())
		return false;

	int iSize = m_oUpdateBuffer.size();
	m_oReceivingThreadMutex.unlock();
	return (iSize > 0);
}

CommunicatorPort_t UdpCommunicator::GetBoundPort()
{
	return m_oUdpSocket.getLocalPort();
}