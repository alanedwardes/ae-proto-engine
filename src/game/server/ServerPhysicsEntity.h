#pragma once

#include "shared\BaseSimulatedEntity.h"
#pragma once

#include "shared\Manifest.h"
#include "shared\INetworked.h"
#include "shared\StreamSerialiser.h"

#include "WorldSimulator.h"

extern WorldSimulator g_oWorldSimulator;

class ServerPhysicsEntity : public BaseSimulatedEntity, public IServerNetworked
{
public:
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			g_oWorldSimulator.CreateDynamicBody(this, pSimulatedBody);
		}
	}

	virtual std::string Serialise()
	{
		StreamSerialiser oSerialiser;
		oSerialiser << position << rotation;
		return oSerialiser.Serialise();
	};
};