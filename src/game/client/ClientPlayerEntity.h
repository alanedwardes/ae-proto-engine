#pragma once

#include "shared\Manifest.h"
#include "shared\INetworked.h"
#include "shared\BasePlayerEntity.h"

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