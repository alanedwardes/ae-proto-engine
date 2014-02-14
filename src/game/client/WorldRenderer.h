#pragma once

#include "sfml\Graphics.hpp"
#include "Point.h"
#include "View.h"
#include "Locator.h"

class IRendered;
class ISimulated;
struct RenderedPolygon;
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
	sf::RenderWindow m_oRenderWindow;
	sf::RenderTarget* m_pWindowTarget;
	sf::Image m_oWindowIcon;
	int m_iDebugFont;
	float m_flZoomLevel;
	Camera m_oCamera;
};