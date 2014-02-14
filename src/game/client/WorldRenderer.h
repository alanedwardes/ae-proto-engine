#pragma once

#include "Point.h"
#include "View.h"
#include "Locator.h"

class IRendered;
class ISimulated;
class BaseGameObject;

class WorldRenderer : public MainView
{
public:
	WorldRenderer();
	virtual void Draw();
	virtual Camera GetCamera() { return m_oCamera; }
private:
	virtual void DrawRenderable(BaseGameObject *pEntity, IRendered* pRenderable);
	virtual void DrawSimulated(BaseGameObject *pEntity, ISimulated* pSimulated);
	virtual void DrawDebugText(BaseGameObject *pEntity);
	int m_iDebugFont;
	float m_flZoomLevel;
	Camera m_oCamera;
};