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
		if (pPoly->textureReference < 0)
		{
			Debug::DebugMessage("Loading texture %", pPoly->texturePath);
			pPoly->textureReference = Locator::Drawing()->LoadTextureResource(pPoly->texturePath);
		}

		const float dumbSmoothFactor = 0.75f;

		pRenderable->lastPosition = (pRenderable->lastPosition * dumbSmoothFactor)
			+ (pEntity->position * (1.0f - dumbSmoothFactor));

		pRenderable->lastRotation = (pRenderable->lastRotation * dumbSmoothFactor)
			+ (pEntity->rotation * (1.0f - dumbSmoothFactor));

		Locator::Drawing()->SetTexture(pPoly->textureReference, pPoly->fill);
		Locator::Drawing()->DrawPolygon(pPoly->polygon, pRenderable->lastPosition,
			pRenderable->lastRotation);
	}
}

void WorldRenderer::DrawSimulated(BaseGameObject* pEntity, ISimulated* pSimulated)
{
	auto oSimulations = pSimulated->GetSimulationData();
	for (auto pSimulated : oSimulations)
	{
		Locator::Drawing()->SetColor(Color(255, 0, 0, 64));
		Locator::Drawing()->DrawPolygon(pSimulated->polygon,
			pEntity->position, pEntity->rotation);
	}
}

void WorldRenderer::Draw()
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