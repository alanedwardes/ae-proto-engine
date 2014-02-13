#pragma once

#include "BaseSimulatedEntity.h"
#include "INetworked.h"
#include "StreamSerialiser.h"

class ClientPhysicsEntity : public BaseSimulatedEntity, public IClientNetworked
{
public:
	virtual void Unserialise(std::string szSerialised)
	{
		StreamSerialiser oSerialiser(szSerialised);
		oSerialiser >> position >> rotation;
	};
};