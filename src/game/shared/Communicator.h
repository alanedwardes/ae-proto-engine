#pragma once

#include "StreamSerialiser.h"

#include <sstream>
#include <mutex>

// Forward declaration
namespace sf { class IpAddress; }

// A port number
typedef unsigned short CommunicatorPort_t;

// Network update payload
typedef StreamSerialiser CommunicatorData_t;

// An IPv4 Address
typedef sf::IpAddress CommunicatorIpV4Address_t;

struct CommunicatorHostAddress_t
{
	CommunicatorIpV4Address_t ipV4Address;
	CommunicatorPort_t port;

	CommunicatorHostAddress_t() : port(0)
	{
		ipV4Address = ipV4Address.None;
	};

	std::string toString()
	{
		std::ostringstream stream;
		stream << ipV4Address.toString() << ":" << port;
		return stream.str();
	};

    inline bool operator==(CommunicatorHostAddress_t other)
	{
        return (ipV4Address.toInteger() == other.ipV4Address.toInteger() &&
			port == other.port);
    };

    inline bool operator!=(CommunicatorHostAddress_t other)
	{
        return (ipV4Address.toInteger() != other.ipV4Address.toInteger() ||
			port != other.port);
    };

    inline bool operator!()
	{
        return (ipV4Address == ipV4Address.None && port <= 0);
    };
};

// A struct containing an IpV4 address, port, and data
struct CommunicatorUpdate_t
{
	unsigned short int seq;
	CommunicatorHostAddress_t host;
	CommunicatorData_t data;
};

class Communicator
{
public:
	// Listen to incoming connections on the specified port
	virtual bool Bind(CommunicatorPort_t port) = 0;

	// Stop listening for incoming connections
	virtual void Unbind() = 0;

	// Returns true if there is data to collect, false if there isn't
	virtual bool Receive() = 0;

	// Returns the port the communicator is listening on or 0
	virtual CommunicatorPort_t GetBoundPort() = 0;

	// Get the last update from a Receive() call
	virtual CommunicatorUpdate_t GetUpdate() = 0;

	// Send a packet to the specified address
	virtual void SendPacket(CommunicatorUpdate_t update) = 0;
};