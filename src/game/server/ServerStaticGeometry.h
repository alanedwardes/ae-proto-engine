#pragma once

#include "shared\BaseSimulatedGeometry.h"
#include "WorldSimulator.h"

extern WorldSimulator g_oWorldSimulator;

class ServerStaticGeometry : public BaseSimulatedGeometry
{
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			g_oWorldSimulator.CreateStaticBody(this, pSimulatedBody);
		}
	}
};