#include "WorldSimulator.h"
#include "ISimulated.h"
#include "BaseGameObject.h"
#include "BaseGameObject.h"

#define _USE_MATH_DEFINES

#include <math.h>

struct SimulationDefinition
{
	ISimulated* simulated;
	BaseGameObject* base;
	SimulatedBody *simBody;
	b2Body* physBody;
};

#define VELOCITY_ITERATIONS 6
#define POSITION_ITERATIONS 2
#define SCALE_MODIFIER 32.0f
#define RADIANS_TO_DEGREES(ANGLE) float(ANGLE / M_PI * 180.0)
#define DEGREES_TO_RADIANS(RADIAN) float(RADIAN * M_PI / 180.0)
#define POINT_FROM_BOX2D(POINT) Point(POINT.x * SCALE_MODIFIER, POINT.y * SCALE_MODIFIER)
#define POINT_TO_BOX2D(POINT) b2Vec2(POINT.x / SCALE_MODIFIER, POINT.y / SCALE_MODIFIER)

WorldSimulator::WorldSimulator()
{
	b2Vec2 gravity(0.0f, 9.81f);
	m_pWorld.reset(new b2World(gravity));
}

void WorldSimulator::AddBodyToSimulatedObjects(SimulationDefinition *pSimulationDefinition)
{
	m_oSimulationDefinitions.push_back(pSimulationDefinition);
}

void WorldSimulator::CreateDynamicBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody)
{
	assert(pEntity);
	auto pSimulated = dynamic_cast<ISimulated*>(pEntity);
	assert(pSimulated);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	b2Body* pBody = m_pWorld->CreateBody(&bodyDef);

	auto oPolygon = CreatePolygon(pSimulatedBody);
	auto oFixtureDef = CreateFixture(pSimulatedBody);
	oFixtureDef.shape = &oPolygon;

	if (pSimulatedBody->fixedRotation)
		pBody->SetFixedRotation(true);

	pBody->SetTransform(POINT_TO_BOX2D(pEntity->position),
		DEGREES_TO_RADIANS(pEntity->rotation));

	pBody->CreateFixture(&oFixtureDef);

	auto pSimulationDefinition = new SimulationDefinition();
	pSimulationDefinition->base = pEntity;
	pSimulationDefinition->physBody = pBody;
	pSimulationDefinition->simBody = pSimulatedBody;
	pSimulationDefinition->simulated = pSimulated;
	pBody->SetUserData(pSimulationDefinition);

	AddBodyToSimulatedObjects(pSimulationDefinition);
}

void WorldSimulator::CreateStaticBody(BaseGameObject *pEntity, SimulatedBody *pSimulatedBody)
{
	assert(pEntity);
	auto pSimulated = dynamic_cast<ISimulated*>(pEntity);
	assert(pSimulated);

	b2BodyDef oStaticBodyDef;
	oStaticBodyDef.type = b2_staticBody;
	oStaticBodyDef.allowSleep = true;

	b2Body *pStaticBody = m_pWorld->CreateBody(&oStaticBodyDef);

	pStaticBody->SetTransform(POINT_TO_BOX2D(pEntity->position),
		DEGREES_TO_RADIANS(pEntity->rotation));

	auto oPolygon = CreatePolygon(pSimulatedBody);
	auto oFixtureDef = CreateFixture(pSimulatedBody);
	oFixtureDef.shape = &oPolygon;

	pStaticBody->CreateFixture(&oFixtureDef);

	auto pSimulationDefinition = new SimulationDefinition();
	pSimulationDefinition->base = pEntity;
	pSimulationDefinition->physBody = pStaticBody;
	pSimulationDefinition->simBody = pSimulatedBody;
	pSimulationDefinition->simulated = pSimulated;
	pStaticBody->SetUserData(pSimulationDefinition);

	AddBodyToSimulatedObjects(pSimulationDefinition);
}

b2PolygonShape WorldSimulator::CreatePolygon(SimulatedBody *pSimulatedBody)
{
	auto oPoints = pSimulatedBody->polygon.points;
	int iPointsNum = oPoints.size();
	b2Vec2 *pPoints = new b2Vec2[iPointsNum];

	// Box2D processes vertices counter-clockwise
	// http://www.box2d.org/manual.html (sec 4.4)
	b2PolygonShape oPolygon;
	int i = iPointsNum;
	while (i--)
	{
		pPoints[i] = POINT_TO_BOX2D(oPoints[i]);
	}
	oPolygon.Set(pPoints, iPointsNum);
	return oPolygon;
}

void WorldSimulator::ApplyForce(unsigned int iBodyId, Point poForce)
{
	auto pBody = m_oSimulationDefinitions[iBodyId]->physBody;
	pBody->ApplyLinearImpulse(POINT_TO_BOX2D(poForce), pBody->GetPosition(), true);
	Debug::DebugMessage("Apply force");
}

b2FixtureDef WorldSimulator::CreateFixture(SimulatedBody *pSimulatedBody)
{
	b2FixtureDef fixtureDef;
	fixtureDef.restitution = pSimulatedBody->restitution;
	fixtureDef.friction = pSimulatedBody->friction;
	fixtureDef.density = pSimulatedBody->mass;
	return fixtureDef;
}

void WorldSimulator::PreSimulate()
{
	for (auto pDef : m_oSimulationDefinitions)
	{
		if (pDef->base->deleted)
		{
			if (pDef->physBody != nullptr)
			{
				m_pWorld->DestroyBody(pDef->physBody);
				pDef->physBody = nullptr;
			}
			continue;
		}

 		pDef->simulated->PreSimulate();
		pDef->physBody->SetLinearVelocity(POINT_TO_BOX2D(pDef->simBody->linearVelocity));
		pDef->physBody->SetTransform(POINT_TO_BOX2D(pDef->base->position),
			DEGREES_TO_RADIANS(pDef->base->rotation));
	}
}

void WorldSimulator::Simulate(float flTimeStep)
{
	PreSimulate();

	m_pWorld->Step(flTimeStep, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

	FindContacts();

	PostSimulate();
}

void WorldSimulator::FindContacts()
{
	for (auto pContact = m_pWorld->GetContactList(); pContact; pContact = pContact->GetNext())
	{
		if (pContact->IsTouching())
		{
			auto pBodyA = pContact->GetFixtureA()->GetBody();
			auto pBodyB = pContact->GetFixtureB()->GetBody();

			auto pSimulationDefA = reinterpret_cast<SimulationDefinition*>(pBodyA->GetUserData());
			auto pSimulationDefB = reinterpret_cast<SimulationDefinition*>(pBodyB->GetUserData());
			pSimulationDefA->simulated->Contact(pSimulationDefB->simulated);
			pSimulationDefB->simulated->Contact(pSimulationDefA->simulated);
		}
	}
}

void WorldSimulator::PostSimulate()
{
	for (auto pDef : m_oSimulationDefinitions)
	{
		if (pDef->base->deleted && pDef->physBody == nullptr)
			continue;

		pDef->base->position = POINT_FROM_BOX2D(pDef->physBody->GetPosition());
		pDef->base->rotation = RADIANS_TO_DEGREES(pDef->physBody->GetAngle());
		pDef->simBody->linearVelocity = POINT_FROM_BOX2D(pDef->physBody->GetLinearVelocity());
		pDef->simulated->PostSimulate();
	}
}