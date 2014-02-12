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
		const float amount = 50;
		const float max = 100.0f;
		int x = 0, y = 0;
		if ((pressedKeys & KEY_UP) && (g_pGameState->time > lastJumpTime + 1000.0f))
		{
			lastJumpTime = g_pGameState->time;
			y = -(amount * 4);
		}
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
				pSimulatedBody->linearVelocity.x = std::max(-max, std::min(pSimulatedBody->linearVelocity.x + x, max));
				pSimulatedBody->linearVelocity.y = pSimulatedBody->linearVelocity.y + y;
			}
		}

		pressedKeys = KEY_NONE;
	};

	virtual void Contact(ISimulated *pSimulated)
	{
		auto pGameObject = dynamic_cast<BaseGameObject*>(pSimulated);

		Debug::Message(pGameObject->DebugText());
	};

	virtual std::string Serialise()
	{
		StreamSerialiser oSerialiser;
		oSerialiser << playerId << position;
		return oSerialiser.Serialise();
	};

	long lastJumpTime;
};