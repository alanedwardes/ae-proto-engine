#pragma once

#include "sfml\Graphics.hpp"
#include "Point.h"
#include "View.h"
#include "Locator.h"

class IRendered;
class ISimulated;
struct RenderedPolygon;
class BaseGameObject;

class WorldRenderer : public View, public IRenderCallback
{
public:
	WorldRenderer();
	virtual void Render();
	virtual Camera GetCamera() { return m_oCamera; }
private:
	virtual void DrawRenderable(BaseGameObject *pEntity, IRendered* pRenderable);
	virtual void DrawSimulated(BaseGameObject *pEntity, ISimulated* pSimulated);
	virtual void DrawDebugText(BaseGameObject *pEntity);
	virtual void AddPointsToShape(sf::ConvexShape *oShape, std::vector<Point> *pPoints);
	virtual sf::Texture* GetTexture(RenderedPolygon *pPolygon);
	sf::RenderWindow m_oRenderWindow;
	sf::RenderTarget* m_pWindowTarget;
	sf::Image m_oWindowIcon;
	int m_iDebugFont;
	float m_flZoomLevel;
	Camera m_oCamera;
	std::vector<sf::Texture*> m_oLoadedTextures;
};