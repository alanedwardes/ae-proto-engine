#pragma once

#include "BaseSimulatedEntity.h"
#include "Locator.h"

class ServerStaticEntity : public BaseSimulatedEntity
{
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			Locator::WorldSimulator()->CreateStaticBody(this, pSimulatedBody);
		}
	}
};