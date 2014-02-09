#pragma once

#include "shared\BaseSimulatedEntity.h"
#include "WorldSimulator.h"

extern WorldSimulator g_oWorldSimulator;

class ServerStaticEntity : public BaseSimulatedEntity
{
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			g_oWorldSimulator.CreateStaticBody(this, pSimulatedBody);
		}
	}
};