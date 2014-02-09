#pragma once

#include "Point.h"
#include "Manifest.h"
#include <vector>

struct RenderedPolygon
{
	RenderedPolygon() : polygonReference(-1) {};

	virtual void LoadManifest(Manifest oRenderingManifest)
	{
		auto oSize = oRenderingManifest.GetPoint("size");
		if (!oSize)
		{
			points = oRenderingManifest.GetPointList("points");
		}
		else
		{
			points.push_back(Point(0, 0));
			points.push_back(Point(0, oSize.y));
			points.push_back(Point(oSize.x, oSize.y));
			points.push_back(Point(oSize.x, 0));
		}

		texturePath = oRenderingManifest.GetFile("texture");
		fill = oRenderingManifest.GetBool("fill");
	};

	bool fill;
	std::vector<Point> points;
	std::string texturePath;
	int polygonReference;
};

class IRendered
{
public:
	virtual std::vector<RenderedPolygon*> GetRenderables() = 0;
	// Position used for client-side interpolation
	Point lastPosition;
	// Rotation used for client-side interpolation
	float lastRotation;
};