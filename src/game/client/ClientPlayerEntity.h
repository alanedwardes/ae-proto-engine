#pragma once

#include "Manifest.h"
#include "INetworked.h"
#include "BasePlayerEntity.h"

#include "ClientUpdateManager.h"

class ClientPlayerEntity : public BasePlayerEntity, public IClientNetworked
{
public:
	virtual void Unserialise(std::string szSerialised)
	{
		StreamSerialiser oSerialiser(szSerialised);
		oSerialiser >> playerId >> position;
	};
};