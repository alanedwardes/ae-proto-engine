#pragma once

#include "Polygon.h"
#include "Color.h"
#include "Locator.h"

struct Camera
{
	Point position;
	float zoom = 0.0f;
	float rotation = 0.0f;
};

class IRenderCallback
{
public:
	virtual void Render() = 0;
	virtual Camera GetCamera() = 0;
};

class IDrawing
{
public:
	virtual void AddRenderCallbackObject(IRenderCallback *pRenderCallback) = 0;
	virtual int LoadFontResource(std::string szFilename) = 0;
	virtual int LoadTextureResource(std::string szFilename) = 0;
	virtual void SetColor(Color color) = 0;
	virtual void SetTexture(int iTextureResource, bool bFill = false) = 0;
	virtual void DrawPolygon(Polygon polygon, Point position, float rotation = 0) = 0;
	virtual void DrawRectangle(Point size, Point position, float rotation = 0) = 0;
	virtual void DrawText(std::string szText, int iFontResource, int iSize, Point position, float rotation = 0) = 0;
	virtual void Render() = 0;
};