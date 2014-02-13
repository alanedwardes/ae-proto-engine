#pragma once

#include "BaseSimulatedEntity.h"
#include "Manifest.h"
#include "INetworked.h"
#include "StreamSerialiser.h"
#include "Locator.h"

#include "WorldSimulator.h"

class ServerPhysicsEntity : public BaseSimulatedEntity, public IServerNetworked
{
public:
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			Locator::WorldSimulator()->CreateDynamicBody(this, pSimulatedBody);
		}
	}

	virtual std::string Serialise()
	{
		StreamSerialiser oSerialiser;
		oSerialiser << position << rotation;
		return oSerialiser.Serialise();
	};
};