#pragma once

#include <vector>
#include "Point.h"

struct Polygon
{
	Polygon(){}
	Polygon(Point poBox)
	{
		points.push_back(Point(0, 0));
		points.push_back(Point(0, poBox.y));
		points.push_back(Point(poBox.x, poBox.y));
		points.push_back(Point(poBox.x, 0));
	}

	std::vector<Point> points;
};