#pragma once

#include <vector>
#include "sfml\Network.hpp"
#include "Point.h"
#include "Vector.h"
#include "zlib\zlib.h"

class StreamSerialiser : public sf::Packet
{
public:
	StreamSerialiser(std::string szData);
	StreamSerialiser() { };

	void Deserialise(std::string szData);
	std::string Serialise();

	StreamSerialiser& operator <<(Point point)
	{
		sf::Packet::operator<<(point.x);
		sf::Packet::operator<<(point.y);
		return *this;
	}

	StreamSerialiser& operator >>(Point &point)
	{
		sf::Packet::operator>>(point.x);
		sf::Packet::operator>>(point.y);
		return *this;
	}

	StreamSerialiser& operator <<(Vector vector)
	{
		sf::Packet::operator<<(vector.x);
		sf::Packet::operator<<(vector.y);
		sf::Packet::operator<<(vector.z);
		return *this;
	}

	StreamSerialiser& operator >>(Vector &vector)
	{
		sf::Packet::operator>>(vector.x);
		sf::Packet::operator>>(vector.y);
		sf::Packet::operator>>(vector.z);
		return *this;
	}

	StreamSerialiser& operator <<(bool bValue)
	{ sf::Packet::operator<<(bValue); return *this; }
	StreamSerialiser& operator >>(bool& bValue)
	{ sf::Packet::operator<<(bValue); return *this; }

	StreamSerialiser& operator <<(float flValue)
	{ sf::Packet::operator<<(flValue); return *this; }
	StreamSerialiser& operator >>(float& flValue)
	{ sf::Packet::operator>>(flValue); return *this; }

	StreamSerialiser& operator <<(double dValue)
	{ sf::Packet::operator<<(dValue); return *this; }
	StreamSerialiser& operator >>(double& dValue)
	{ sf::Packet::operator>>(dValue); return *this; }

	StreamSerialiser& operator <<(const char* cValue)
	{ sf::Packet::operator<<(cValue); return *this; }
	StreamSerialiser& operator >>(char* cValue)
	{ sf::Packet::operator>>(cValue); return *this; }

	StreamSerialiser& operator <<(sf::Int8 iValue)
	{ sf::Packet::operator<<(iValue); return *this; }
	StreamSerialiser& operator >>(sf::Int8& iValue)
	{ sf::Packet::operator>>(iValue); return *this; }

	StreamSerialiser& operator <<(sf::Uint8 iValue)
	{ sf::Packet::operator<<(iValue); return *this; }
	StreamSerialiser& operator >>(sf::Uint8& iValue)
	{ sf::Packet::operator>>(iValue); return *this; }

	StreamSerialiser& operator <<(sf::Int16 iValue)
	{ sf::Packet::operator<<(iValue); return *this; }
	StreamSerialiser& operator >>(sf::Int16& iValue)
	{ sf::Packet::operator>>(iValue); return *this; }

	StreamSerialiser& operator <<(sf::Uint16 iValue)
	{ sf::Packet::operator<<(iValue); return *this; }
	StreamSerialiser& operator >>(sf::Uint16& iValue)
	{ sf::Packet::operator>>(iValue); return *this; }

	StreamSerialiser& operator <<(sf::Int32 iValue)
	{ sf::Packet::operator<<(iValue); return *this; }
	StreamSerialiser& operator >>(sf::Int32& iValue)
	{ sf::Packet::operator>>(iValue); return *this; }

	StreamSerialiser& operator <<(sf::Uint32 iValue)
	{ sf::Packet::operator<<(iValue); return *this; }
	StreamSerialiser& operator >>(sf::Uint32& iValue)
	{ sf::Packet::operator>>(iValue); return *this; }

	StreamSerialiser& operator <<(std::string szValue)
	{ sf::Packet::operator<<(szValue); return *this; }
	StreamSerialiser& operator >>(std::string& szValue)
	{ sf::Packet::operator>>(szValue); return *this; }

private:
	virtual const void* onSend(std::size_t& size);
	virtual void onReceive(const void* data, std::size_t size);
	std::vector<Bytef> m_oCompressionBuffer;
};