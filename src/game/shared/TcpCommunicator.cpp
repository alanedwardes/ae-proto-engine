#include "TcpCommunicator.h"

TcpCommunicator::TcpCommunicator()
{
}

TcpCommunicator::~TcpCommunicator()
{
	// Stop listening to the port
	Unbind();
}

bool TcpCommunicator::Bind(CommunicatorPort_t port)
{
	if (m_pListeningTcpSocket != nullptr)
		return false;

	sf::Socket::Status status = m_oTcpListener.listen(port);

	if (status == sf::Socket::Done)
		Debug::DebugMessage("Socket listening on port %", port);
	else
		Debug::WarningMessage("Error (%) binding to port %", status, port);

	if (status != sf::Socket::Done)
		return false;

	// Start a listening thread
	new std::thread(&ListenThread, &m_oTcpListener, &m_oUpdateBuffer,
		&m_oReceivingThreadMutex, &m_oClientSockets, &m_oListeningThreadMutex);

	return true;
}

void TcpCommunicator::Connect(CommunicatorHostAddress_t host)
{
	m_pListeningTcpSocket = new sf::TcpSocket();
	sf::Socket::Status status = m_pListeningTcpSocket->connect(host.ipV4Address, host.port);

	if (status == sf::Socket::Done)
		Debug::DebugMessage("Connected to %:%", host.ipV4Address, host.port);
	else
		Debug::WarningMessage("Error (%) connecting to %:%", status,
		host.ipV4Address, host.port);

	new std::thread(&ReceiveThread, m_pListeningTcpSocket,
		&m_oUpdateBuffer, &m_oListeningThreadMutex);
}

void TcpCommunicator::Unbind()
{
	m_oTcpListener.close();
}

void TcpCommunicator::SendPacket(CommunicatorUpdate_t update)
{
	if (m_pListeningTcpSocket != nullptr)
	{
		Debug::DebugMessage("Sending % bytes to socket", update.data.getDataSize());

		if (m_pListeningTcpSocket->send(update.data) != sf::Socket::Done)
		{
			Debug::DebugMessage("SendPacket(c)::Socket disconnected");
		}
	}
	else
	{
		// Exclusive access to the socket vector
		m_oListeningThreadMutex.lock();

		for (auto pTcpSocket : m_oClientSockets)
		{
			auto address = pTcpSocket->getRemoteAddress();
			auto port = pTcpSocket->getRemotePort();

			if (address == update.host.ipV4Address &&
				port == update.host.port)
			{
				Debug::DebugMessage("Sending % bytes to socket", update.data.getDataSize());

				if (pTcpSocket->send(update.data) != sf::Socket::Done)
				{
					Debug::DebugMessage("SendPacket::Socket disconnected");
				}
			}
		}

		m_oListeningThreadMutex.unlock();
	}
}

void TcpCommunicator::ListenThread(sf::TcpListener *pListener,
	std::vector<CommunicatorUpdate_t> *pUpdateBuffer,
	std::mutex *pReceivingThreadMutex,
	std::vector<sf::TcpSocket*> *pClientSockets,
	std::mutex *pListeningThreadMutex)
{
	do
	{
		sf::TcpSocket *pSocket = new sf::TcpSocket();
		pListener->accept(*pSocket);
		Debug::DebugMessage("Client %:% connected", pSocket->getRemoteAddress().toString(),
			pSocket->getLocalPort());

		pListeningThreadMutex->lock();
		pClientSockets->push_back(pSocket);
		pListeningThreadMutex->unlock();

		// Start a receiving thread
		new std::thread(&ReceiveThread, pSocket,
			pUpdateBuffer, pReceivingThreadMutex);
	}
	while (true);
}

void TcpCommunicator::ReceiveThread(sf::TcpSocket *pTcpSocket,
		std::vector<CommunicatorUpdate_t> *pUpdateBuffer,
		std::mutex *pReceivingThreadMutex)
{
	Debug::DebugMessage("Starting listening thread");

	sf::Socket::Status status;

	do
	{
		CommunicatorData_t data;

		status = pTcpSocket->receive(data);
		if (status != sf::Socket::Done)
		{
			Debug::WarningMessage("ReceiveThread::Socket disconnected (%)", status);
		}
		else
		{
			CommunicatorUpdate_t oUpdate;
			oUpdate.data = data;
			oUpdate.host.ipV4Address = pTcpSocket->getRemoteAddress();
			oUpdate.host.port = pTcpSocket->getRemotePort();

			pReceivingThreadMutex->lock();
			pUpdateBuffer->push_back(oUpdate);
			pReceivingThreadMutex->unlock();
		}
	} while(status == sf::Socket::Done);

	Debug::DebugMessage("Exiting listening thread");
}

CommunicatorUpdate_t TcpCommunicator::GetUpdate()
{
	m_oReceivingThreadMutex.lock();
	auto oUpdate = m_oUpdateBuffer.back();
	m_oUpdateBuffer.pop_back();
	m_oReceivingThreadMutex.unlock();
	return oUpdate;
}

bool TcpCommunicator::Receive()
{
	if (!m_oReceivingThreadMutex.try_lock())
		return false;

	int iSize = m_oUpdateBuffer.size();
	m_oReceivingThreadMutex.unlock();
	return (iSize > 0);
}

CommunicatorPort_t TcpCommunicator::GetBoundPort()
{
	return m_oTcpListener.getLocalPort();
}