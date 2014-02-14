#pragma once

#include "Polygon.h"
#include "Color.h"
#include "Locator.h"

class IDrawing
{
public:
	virtual int LoadFontResource(std::string szFilename) = 0;
	virtual int LoadTextureResource(std::string szFilename) = 0;
	virtual void SetColor(Color color) = 0;
	virtual void SetTexture(int iTextureResource, bool bFill = false) = 0;
	virtual void DrawPolygon(Polygon polygon, Point position, float rotation = 0) = 0;
	virtual void DrawRectangle(Point size, Point position, float rotation = 0) = 0;
	virtual void DrawText(std::string szText, int iFontResource, int iSize, Point position, float rotation = 0) = 0;
};