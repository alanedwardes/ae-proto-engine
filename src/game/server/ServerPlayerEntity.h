#pragma once

#include "Manifest.h"
#include "INetworked.h"
#include "BasePlayerEntity.h"

#include "WorldSimulator.h"
#include "Key.h"
#include "Locator.h"

class ServerPlayerEntity : public BasePlayerEntity, public IServerNetworked
{
public:
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			Locator::WorldSimulator()->CreateDynamicBody(this, pSimulatedBody);
		}
	};

	virtual void PreSimulate()
	{
		const float amount = 50;
		const float max = 300.0f;
		float x = 0, y = 0;
		if (pressedKeys & KEY_UP)
		{
			y = -(amount * 4);
		}
		if (pressedKeys & KEY_RIGHT)
			x = amount;
		if (pressedKeys & KEY_DOWN)
			y = amount;
		if (pressedKeys & KEY_LEFT)
			x = -amount;

		if (x || y)
		{
			for (auto pSimulatedBody : GetSimulationData())
			{
				pSimulatedBody->linearVelocity.x = std::max(-max, std::min(pSimulatedBody->linearVelocity.x + x, max));
				pSimulatedBody->linearVelocity.y = std::max(-max, std::min(pSimulatedBody->linearVelocity.y + y, max));
			}
		}
	};

	//virtual void Contact(ISimulated *pSimulated)
	//{
	//	//auto pGameObject = dynamic_cast<BaseGameObject*>(pSimulated);

	//	//Debug::Message(pGameObject->DebugText());
	//};

	virtual std::string Serialise()
	{
		StreamSerialiser oSerialiser;
		oSerialiser << playerId << position;
		return oSerialiser.Serialise();
	};
};