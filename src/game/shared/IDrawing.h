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
	virtual int LoadFontResource(std::string szFilename) = 0;
	virtual void AddRenderCallbackObject(IRenderCallback *pRenderCallback) = 0;
	virtual void SetColor(Color color) = 0;
	virtual void DrawRectangle(Point size, Point position) = 0;
	virtual void DrawText(std::string szText, int iFontResource, int iSize, Point position) = 0;
	virtual void Render() = 0;
};