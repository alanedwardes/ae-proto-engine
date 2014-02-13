#pragma once

#include "Polygon.h"

class IDrawing
{
public:
	virtual void DrawPolygon(Polygon *pPolygon) = 0;
};