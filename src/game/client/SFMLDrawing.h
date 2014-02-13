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

	virtual void SetTexture(int iTextureResource, bool bFill)
	{
		m_iLastTextureResource = iTextureResource;
		m_bLastTextureFillState = bFill;
	}

	virtual void DrawRectangle(Point size, Point position, float rotation)
	{
		sf::RectangleShape oShape(POINT_TO_SFML(size));
		oShape.setPosition(POINT_TO_SFML(position));
		oShape.setRotation(rotation);
		oShape.setFillColor(COLOR_TO_SFML(m_cLastColor));

		if (m_iLastTextureResource > -1)
		{
			oShape.setTexture(m_oTextureResources[m_iLastTextureResource]);

			if (!m_bLastTextureFillState)
			{
				auto poSize = oShape.getLocalBounds();
				oShape.setTextureRect(sf::IntRect(0, 0,
					int(poSize.width), int(poSize.height)));
			}
		}

		DrawShape(&oShape);
	}

	virtual void DrawPolygon(Polygon polygon, Point position, float rotation)
	{
		sf::ConvexShape oShape;
		int iNumPoints = polygon.points.size();
		oShape.setPointCount(iNumPoints);
		oShape.setRotation(rotation);
		oShape.setPosition(POINT_TO_SFML(position));
		for (int i = 0; i < iNumPoints; i++)
		{
			Point poPoint = polygon.points.at(i);
			oShape.setPoint(i, POINT_TO_SFML(poPoint));
		}
		oShape.setFillColor(COLOR_TO_SFML(m_cLastColor));

		if (m_iLastTextureResource > -1)
		{
			oShape.setTexture(m_oTextureResources[m_iLastTextureResource]);

			if (!m_bLastTextureFillState)
			{
				auto poSize = oShape.getLocalBounds();
				oShape.setTextureRect(sf::IntRect(0, 0,
					int(poSize.width), int(poSize.height)));
			}
		}

		DrawShape(&oShape);
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

	virtual int LoadTextureResource(std::string szFilename)
	{
		auto pTexture = new sf::Texture();
		pTexture->loadFromFile(szFilename);
		pTexture->setSmooth(true);
		auto iTextureIndex = m_oTextureResources.size();
		m_oTextureResources.push_back(pTexture);
		return iTextureIndex;
	}

	virtual void DrawText(std::string szText, int iFontResource, int iSize, Point position, float rotation)
	{
		sf::Text text;
		text.setFont(*m_oFontResources[iFontResource]);
		text.setCharacterSize(iSize);
		text.setString(szText);
		text.setRotation(rotation);
		text.setPosition(POINT_TO_SFML(position));
		text.setColor(COLOR_TO_SFML(m_cLastColor));
		DrawShape(&text);
	}

	virtual void SetRenderTarget(sf::RenderTarget *pRT){ m_pRT = pRT; }

private:
	virtual void DrawShape(sf::Drawable *pDrawable)
	{
		m_pRT->draw(*pDrawable);
		m_cLastColor = Color();
		m_iLastTextureResource = -1;
		m_bLastTextureFillState = false;
	}
	Color m_cLastColor;
	int m_iLastTextureResource = -1;
	bool m_bLastTextureFillState = false;
	std::vector<IRenderCallback*> m_oRenderers;
	std::vector<sf::Texture*> m_oTextureResources;
	std::vector<sf::Font*> m_oFontResources;
	sf::RenderTarget *m_pRT;
};