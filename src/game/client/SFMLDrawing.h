#pragma once

#include "IDrawing.h"
#include "Locator.h"
#include "sfml\Graphics.hpp"

#define POINT_TO_SFML(POINT) sf::Vector2f(POINT.x, POINT.y)
#define POINT_FROM_SFML(POINT) Point(POINT.x, POINT.y)
#define COLOR_TO_SFML(COLOR) sf::Color(COLOR.r, COLOR.g, COLOR.b, COLOR.a)

class SFMLDrawing : public IDrawing
{
public:
	virtual void SetColor(Color color)
	{
		m_cLastColor = color;
	}

	virtual void DrawShape(sf::Drawable *pDrawable)
	{
		m_pRT->draw(*pDrawable);
	}

	virtual void DrawRectangle(Point size, Point position)
	{
		sf::RectangleShape shape(POINT_TO_SFML(size));
		shape.setPosition(POINT_TO_SFML(position));
		shape.setFillColor(COLOR_TO_SFML(m_cLastColor));
		DrawShape(&shape);
	}

	virtual void AddRenderCallbackObject(IRenderCallback *pRenderCallback)
	{
		m_oRenderers.push_back(pRenderCallback);
	}

	virtual void Render()
	{
		for (auto pRenderer : m_oRenderers)
		{
			auto oCamera = pRenderer->GetCamera();
			auto oView = sf::View();
			oView.setRotation(oCamera.rotation);
			oView.zoom(oCamera.zoom);
			oView.setCenter(POINT_TO_SFML(oCamera.position));
			auto pSize = m_pRT->getSize();
			oView.setSize(pSize.x, pSize.y);
			m_pRT->setView(oView);

			pRenderer->Render();
		}
	}

	virtual int LoadFontResource(std::string szFilename)
	{
		auto pFont = new sf::Font();
		pFont->loadFromFile(szFilename);
		auto iFontIndex = m_oFontResources.size();
		m_oFontResources.push_back(pFont);
		return iFontIndex;
	}

	virtual void DrawText(std::string szText, int iFontResource, int iSize, Point position)
	{
		sf::Text text;
		text.setFont(*m_oFontResources[iFontResource]);
		text.setCharacterSize(iSize);
		text.setString(szText);
		text.setPosition(POINT_TO_SFML(position));
		text.setColor(COLOR_TO_SFML(m_cLastColor));
		DrawShape(&text);
	}

	virtual void SetRenderTarget(sf::RenderTarget *pRT){ m_pRT = pRT; }
private:
	Color m_cLastColor;
	std::vector<IRenderCallback*> m_oRenderers;
	std::vector<sf::Font*> m_oFontResources;
	sf::RenderTarget *m_pRT;
};