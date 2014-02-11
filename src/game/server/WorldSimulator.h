#pragma once

#include <vector>
#include "shared\ISimulated.h"
#include "box2d\Box2D.h"

class BaseGameObject;
struct SimulationDefinition;

class WorldSimulator
{
public:
	WorldSimulator();
	virtual void Simulate(float flTimeStep);
	virtual unsigned int CreateDynamicBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody);
	virtual void CreateStaticBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody);
	virtual void ApplyForce(unsigned int iBodyId, Point poForce);
private:
	virtual void PreSimulate();
	virtual void PostSimulate();

	unsigned int AddBodyToDynamicSimulatedObjects(SimulationDefinition *pSimulationDefinition)
	{
		unsigned int iIndex = m_oSimulationDefinitions.size();
		m_oSimulationDefinitions.push_back(pSimulationDefinition);
		return iIndex;
	};
	b2PolygonShape CreatePolygon(SimulatedBody *pSimulatedBody);
	b2Body CreateBody(BaseGameObject *pEntity);
	b2FixtureDef CreateFixture(SimulatedBody *pSimulatedBody);
	std::vector<SimulationDefinition*> m_oSimulationDefinitions;
	std::unique_ptr<b2World> m_pWorld;
};