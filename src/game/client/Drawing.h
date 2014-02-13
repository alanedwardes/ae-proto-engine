#pragma once

#include "IDrawing.h"
#include "Locator.h"
#include "sfml\Graphics.hpp"

class Drawing : public IDrawing
{
public:

	virtual void DrawPolygon(Polygon *pPolygon)
	{

	}

	virtual void Render()
	{

	}

	virtual void SetRenderTarget(sf::RenderTarget *pRT){ m_pRT = pRT; }
private:
	sf::RenderTarget *m_pRT;
};