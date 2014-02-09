#pragma once

#include <vector>
#include "shared\ISimulated.h"

class b2World;
class b2Body;
struct b2FixtureDef;
class b2PolygonShape;
class BaseGameObject;
struct SimulationDefinition;

class WorldSimulator
{
public:
	WorldSimulator();
	~WorldSimulator();
	virtual void Simulate(float flTimeStep);
	virtual unsigned int CreateDynamicBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody);
	virtual void CreateStaticBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody);
	virtual void ApplyForce(unsigned int iBodyId, Point poForce);
private:
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
	b2World *pWorld;
};