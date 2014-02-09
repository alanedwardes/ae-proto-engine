#pragma once

#include "shared\BaseSimulatedEntity.h"
#include "shared\INetworked.h"
#include "shared\StreamSerialiser.h"

class ClientPhysicsEntity : public BaseSimulatedEntity, public IClientNetworked
{
public:
	virtual void Unserialise(std::string szSerialised)
	{
		StreamSerialiser oSerialiser(szSerialised);
		oSerialiser >> position >> rotation;
	};
};