#include "WorldRenderer.h"
#include "Manifest.h"
#include "BaseGameObject.h"
#include "IRendered.h"
#include "ClientPlayerEntity.h"
#include "InputManager.h"
#include "IWorldManager.h"
#include "Locator.h"
#include "GameState.h"
#include "SFMLDrawing.h"

WorldRenderer::WorldRenderer()
{
	auto szDebugFont = Locator::GameState()->Settings()->GetFile("debug_font");
	m_iDebugFont = Locator::Drawing()->LoadFontResource(szDebugFont);
	Locator::Drawing()->AddRenderCallbackObject(this);
}

sf::Texture* WorldRenderer::GetTexture(RenderedPolygon *pPolygon)
{
	auto iTextureIndex = pPolygon->polygonReference;
	if (iTextureIndex >= m_oLoadedTextures.size() || iTextureIndex < 0)
	{
		auto pTexture = new sf::Texture();
		pTexture->setSmooth(true);
		pTexture->setRepeated(true);
		pTexture->loadFromFile(pPolygon->texturePath);
		pPolygon->polygonReference = m_oLoadedTextures.size();
		m_oLoadedTextures.push_back(pTexture);
		return pTexture;
	}
	else
	{
		return m_oLoadedTextures[iTextureIndex];
	}
}

void WorldRenderer::DrawDebugText(BaseGameObject *pEntity)
{
	Locator::Drawing()->SetColor(Color(0, 0, 0));
	Locator::Drawing()->DrawText(pEntity->DebugText(), m_iDebugFont, 11, pEntity->position);
}

void WorldRenderer::DrawRenderable(BaseGameObject *pEntity, IRendered *pRenderable)
{
	auto oRenderables = pRenderable->GetRenderables();
	for (auto pPoly : oRenderables)
	{
		const float dumbSmoothFactor = 0.75f;

		sf::ConvexShape oShape;
		AddPointsToShape(&oShape, &pPoly->polygon.points);

		Point poPosition = (pRenderable->lastPosition * dumbSmoothFactor) + (pEntity->position * (1.0f - dumbSmoothFactor));
		oShape.setPosition(POINT_TO_SFML(poPosition));
		pRenderable->lastPosition = poPosition;

		float flRoation = (pRenderable->lastRotation * dumbSmoothFactor) + (pEntity->rotation * (1.0f - dumbSmoothFactor));
		oShape.setRotation(flRoation);
		pRenderable->lastRotation = flRoation;

		auto pTexture = GetTexture(pPoly);
		oShape.setTexture(pTexture);

		if (!pPoly->fill)
		{
			auto poSize = oShape.getLocalBounds();
			oShape.setTextureRect(sf::IntRect(0, 0,
				int(poSize.width), int(poSize.height)));
		}

		((SFMLDrawing*)Locator::Drawing())->DrawShape(&oShape);
	}
}

void WorldRenderer::DrawSimulated(BaseGameObject* pEntity, ISimulated* pSimulated)
{
	auto oSimulations = pSimulated->GetSimulationData();
	for (auto pSimulated : oSimulations)
	{
		sf::ConvexShape oShape;
		AddPointsToShape(&oShape, &pSimulated->polygon.points);

		oShape.setPosition(POINT_TO_SFML(pEntity->position));
		oShape.setRotation(pEntity->rotation);

		oShape.setFillColor(sf::Color(255, 0, 0, 64));
		oShape.setOutlineColor(sf::Color(255, 0, 0, 192));
		oShape.setOutlineThickness(1.0f);

		((SFMLDrawing*)Locator::Drawing())->DrawShape(&oShape);
	}
}

void WorldRenderer::AddPointsToShape(sf::ConvexShape *oShape, std::vector<Point> *pPoints)
{
	int iNumPoints = pPoints->size();
	oShape->setPointCount(iNumPoints);
	for (int i = 0; i < iNumPoints; i++)
	{
		Point poPoint = pPoints->at(i);
		oShape->setPoint(i, POINT_TO_SFML(poPoint));
	}
}

void WorldRenderer::Render()
{
	auto oEntities = Locator::WorldManager()->GetGameObjects();
	for (auto pEntity : oEntities)
	{
		if (pEntity->deleted)
			continue;

		auto pRenderable = dynamic_cast<IRendered*>(pEntity);
		if (pRenderable)
		{
			auto pPlayer = (ClientPlayerEntity*)pEntity;
			if (pPlayer && Locator::GameState()->UpdateClientId() == pPlayer->playerId)
			{
				// Smooth the camera position - use 90% of the old, 10% of the entity position
				// Given time, this is smoothed out. TODO: make this use delta time
				m_oCamera.position = (m_oCamera.position * .9f) + (pPlayer->position * .1f);
			}

			DrawRenderable(pEntity, pRenderable);
		}
	}

	if (Locator::GameState()->Settings()->GetBool("debug"))
	{
		for (auto pEntity : oEntities)
		{
			if (pEntity->deleted)
				continue;

			DrawDebugText(pEntity);
			auto pSimulated = dynamic_cast<ISimulated*>(pEntity);
			if (pSimulated)
				DrawSimulated(pEntity, pSimulated);
		}
	}
}