#pragma once

#include "Manifest.h"
#include "Point.h"
#include <vector>

struct SimulatedBody
{
	virtual void LoadManifest(Manifest oSimulationManifest)
	{
		auto oSize = oSimulationManifest.GetPoint("size");
		if (!oSize)
		{
			polygon = oSimulationManifest.GetPolygon("points");
		}
		else
		{
			polygon = Polygon(oSize);
		}

		friction = oSimulationManifest.GetFloat("friction");
		restitution = oSimulationManifest.GetFloat("restitution");
		mass = oSimulationManifest.GetFloat("mass");
		fixedRotation = oSimulationManifest.GetBool("fixed_rotation");
	};

	bool fixedRotation;
	float mass;
	float restitution;
	float friction;
	Point linearVelocity;
	Polygon polygon;
};

class ISimulated
{
public:
	virtual std::vector<SimulatedBody*> GetSimulationData() = 0;
	virtual void PreSimulate() = 0;
	virtual void PostSimulate() = 0;
	virtual void Contact(ISimulated *pSimulated) = 0;
};