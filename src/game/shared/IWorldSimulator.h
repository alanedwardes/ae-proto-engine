#pragma once

#include "ISimulated.h"
#include "BaseGameObject.h"

class IWorldSimulator
{
public:
	virtual void CreateDynamicBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody) = 0;
	virtual void CreateStaticBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody) = 0;
};