#pragma once

#include <vector>
#include "ISimulated.h"
#include "box2d\Box2D.h"
#include "IWorldSimulator.h"

class BaseGameObject;
struct SimulationDefinition;

class WorldSimulator : public IWorldSimulator
{
public:
	WorldSimulator();
	virtual void Simulate(float flTimeStep);
	virtual void CreateDynamicBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody);
	virtual void CreateStaticBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody);
	virtual void ApplyForce(unsigned int iBodyId, Point poForce);
private:
	virtual void PreSimulate();
	virtual void FindContacts();
	virtual void PostSimulate();
	virtual void AddBodyToSimulatedObjects(SimulationDefinition *pSimulationDefinition);
	b2PolygonShape CreatePolygon(SimulatedBody *pSimulatedBody);
	b2Body CreateBody(BaseGameObject *pEntity);
	b2FixtureDef CreateFixture(SimulatedBody *pSimulatedBody);
	std::vector<SimulationDefinition*> m_oSimulationDefinitions;
	std::unique_ptr<b2World> m_pWorld;
};