#pragma once

#include "BaseSimulatedGeometry.h"
#include "WorldSimulator.h"
#include "Locator.h"

class ServerStaticGeometry : public BaseSimulatedGeometry
{
	virtual void Init()
	{
		for (auto pSimulatedBody : GetSimulationData())
		{
			Locator::WorldSimulator()->CreateStaticBody(this, pSimulatedBody);
		}
	}
};