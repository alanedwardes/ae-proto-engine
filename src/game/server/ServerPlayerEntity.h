#pragma once

#include "shared\Manifest.h"
#include "shared\INetworked.h"
#include "shared\BasePlayerEntity.h"

#include "WorldSimulator.h"
#include "shared\Key.h"

extern WorldSimulator g_oWorldSimulator;

class ServerPlayerEntity : public BasePlayerEntity, public IServerNetworked
{
public:
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			g_oWorldSimulator.CreateDynamicBody(this, pSimulatedBody);
		}
	};

	virtual void PreSimulate()
	{
		const int amount = 10;
		const int max = 100;
		int x = 0, y = 0;
		if (pressedKeys & KEY_UP)
			y = -amount;
		if (pressedKeys & KEY_RIGHT)
			x = amount;
		if (pressedKeys & KEY_DOWN)
			y = amount;
		if (pressedKeys & KEY_LEFT)
			x = -amount;

		if (x != 0 || y != 0)
		{
			for (auto pSimulatedBody : GetSimulationData())
			{
 				pSimulatedBody->linearVelocity.x += std::min(x, max);
          		pSimulatedBody->linearVelocity.y += std::min(y, max);
			}
		}
	};

	virtual std::string Serialise()
	{
		StreamSerialiser oSerialiser;
		oSerialiser << playerId << position;
		return oSerialiser.Serialise();
	};

	int bodyId;
};