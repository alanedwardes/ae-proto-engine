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
			polygon = oRenderingManifest.GetPolygon("points");
		}
		else
		{
			polygon = Polygon(oSize);
		}

		texturePath = oRenderingManifest.GetFile("texture");
		fill = oRenderingManifest.GetBool("fill");
	};

	bool fill;
	std::string texturePath;
	int polygonReference;
	Polygon polygon;
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