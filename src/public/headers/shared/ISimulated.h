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
			points = oSimulationManifest.GetPointList("points");
		}
		else
		{
			points.push_back(Point(0, 0));
			points.push_back(Point(0, oSize.y));
			points.push_back(Point(oSize.x, oSize.y));
			points.push_back(Point(oSize.x, 0));
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
	std::vector<Point> points;
};

class ISimulated
{
public:
	virtual std::vector<SimulatedBody*> GetSimulationData() = 0;
	virtual void PreSimulate() = 0;
	virtual void PostSimulate() = 0;
	int bodyId;
};