#pragma once

#include "sfml\Graphics.hpp"
#include "shared\Point.h"

class IRendered;
class ISimulated;
struct RenderedPolygon;
class BaseGameObject;

class WorldRenderer
{
public:
	WorldRenderer();
	~WorldRenderer();
	virtual void Render();
	virtual void ProcessEvents();
private:
	virtual void ResetView();
	virtual void GetWindowIcon();
	virtual void RenderEntities();
	virtual void DrawRenderable(BaseGameObject *pEntity, IRendered* pRenderable);
	virtual void DrawSimulated(BaseGameObject *pEntity, ISimulated* pSimulated);
	virtual void DrawDebugText(BaseGameObject *pEntity);
	virtual void AddPointsToShape(sf::ConvexShape *oShape, std::vector<Point> *pPoints);
	virtual sf::Texture* GetTexture(RenderedPolygon *pPolygon);
	sf::RenderWindow m_oRenderWindow;
	sf::RenderTarget* m_pWindowTarget;
	sf::Image m_oWindowIcon;
	sf::Font m_oDebugFont;
	float m_flZoomLevel;
	sf::View m_oMainGameView;
	sf::View m_oUiView;
	std::vector<sf::Texture*> m_oLoadedTextures;
};