#include "shared\WorldManager.h"
#include "WorldRenderer.h"
#include "shared\Manifest.h"
#include "shared\BaseGameObject.h"
#include "shared\IRendered.h"
#include "ClientPlayerEntity.h"
#include "InputManager.h"

extern bool g_bClientRunning;
extern WorldManager g_oWorldManager;
extern Manifest *g_pSettings;
extern ClientUpdateManager g_oClientUpdateManager;
extern InputManager *g_pInputManager;

#define POINT_TO_SFML(POINT) sf::Vector2f(POINT.x, POINT.y)
#define POINT_FROM_SFML(POINT) Point(POINT.x, POINT.y)

WorldRenderer::WorldRenderer()
{
	sf::ContextSettings oContextSettings = m_oRenderWindow.getSettings();
	oContextSettings.antialiasingLevel = g_pSettings->GetInt("client_antialias");

	Point poSize = g_pSettings->GetPoint("client_size", Point(800, 600));

	m_pWindowTarget = &m_oRenderWindow;

	m_oRenderWindow.create(sf::VideoMode(int(poSize.x), int(poSize.y)), "Waiting...",
		sf::Style::Default, oContextSettings);

	m_oRenderWindow.setFramerateLimit(g_pSettings->GetInt("fps_limit", 60));

	auto szDebugFont = g_pSettings->GetFile("debug_font");
	m_oDebugFont.loadFromFile(szDebugFont);

	ResetView();

	GetWindowIcon();
}

WorldRenderer::~WorldRenderer()
{
	m_oRenderWindow.close();
}

void WorldRenderer::GetWindowIcon()
{
	auto szIconPath = g_pSettings->GetFile("client_icon");
	
	if (szIconPath.length() == 0)
		return;

	if (!m_oWindowIcon.loadFromFile(szIconPath))
		return;
	
	sf::Vector2u oSize = m_oWindowIcon.getSize();

	if (oSize.x <= 0 || oSize.y <= 0)
		return;

	m_oRenderWindow.setIcon(oSize.x, oSize.y, m_oWindowIcon.getPixelsPtr());
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

void WorldRenderer::RenderEntities()
{
	auto oEntities = g_oWorldManager.GetGameObjects();
	for (auto pEntity : oEntities)
	{
		if (pEntity->deleted)
			continue;

		auto pRenderable = dynamic_cast<IRendered*>(pEntity);
		if (pRenderable)
		{
			auto pPlayer = (ClientPlayerEntity*)pEntity;
			if (pPlayer && g_oClientUpdateManager.GetUpdateClientId() == pPlayer->playerId)
			{
				// Smooth the camera position
				Point poViewCenter = (POINT_FROM_SFML(m_oMainGameView.getCenter()) * 0.9f) + (pPlayer->position * 0.1f);
				m_oMainGameView.setCenter(POINT_TO_SFML(poViewCenter));
				m_oMainGameView.zoom(m_flZoomLevel);
				m_oMainGameView.setRotation(pPlayer->rotation);
			}

			DrawRenderable(pEntity, pRenderable);
		}
	}

	if (g_pSettings->GetBool("debug"))
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

void WorldRenderer::DrawDebugText(BaseGameObject *pEntity)
{
	sf::Text oText;
	oText.setPosition(POINT_TO_SFML(pEntity->position));
	oText.setString(pEntity->DebugText());
	oText.setCharacterSize(11);
	oText.setColor(sf::Color::Black);
	oText.setFont(m_oDebugFont);
	m_oRenderWindow.draw(oText);
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

		m_oRenderWindow.draw(oShape);
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

		m_oRenderWindow.draw(oShape);
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
	m_oRenderWindow.clear(sf::Color::White);

	m_oRenderWindow.setView(m_oMainGameView);

	RenderEntities();

	if (g_oWorldManager.IsLevelLoaded())
	{
		m_oRenderWindow.setTitle(g_oWorldManager.LevelName());
	}

	m_flZoomLevel = 1.0f;

    m_oRenderWindow.display();
}

void WorldRenderer::ProcessEvents()
{
	sf::Event event;
    while (m_oRenderWindow.pollEvent(event))
    {
		sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
		auto pos = m_pWindowTarget->mapPixelToCoords(mousePos, m_oMainGameView);
		g_pInputManager->NewMousePosition(POINT_FROM_SFML(pos));

        if (event.type == sf::Event::Closed)
			g_bClientRunning = false;

		if (event.type == sf::Event::Resized)
			ResetView();

		Key ePressedKey = KEY_NONE;
		if (event.type == sf::Event::KeyPressed)
		{
			g_pInputManager->KeyPress(
				g_pInputManager->TranslateKeyCode(event.key.code));

			if (event.key.code == sf::Keyboard::F5)
			{
				g_pSettings->SetBool("debug", !g_pSettings->GetBool("debug"));
				g_pSettings->WriteManifest();
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			g_pInputManager->KeyPress(
				g_pInputManager->TranslateKeyCode(event.mouseButton.button));
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			g_pInputManager->KeyRelease(
				g_pInputManager->TranslateKeyCode(event.mouseButton.button));
		}

		if (event.type == sf::Event::MouseWheelMoved)
		{
			m_flZoomLevel -= event.mouseWheel.delta / 10.0f;
		}

		if (event.type == sf::Event::KeyReleased)
		{
			g_pInputManager->KeyRelease(
				g_pInputManager->TranslateKeyCode(event.key.code));
		}
    }
}

void WorldRenderer::ResetView()
{
	auto oWindowSize = m_oRenderWindow.getSize();
	g_pSettings->SetPoint("client_size", POINT_FROM_SFML(oWindowSize));
	g_pSettings->WriteManifest();

	m_oMainGameView.setSize(float(oWindowSize.x), float(oWindowSize.y));
	m_oUiView.setSize(float(oWindowSize.x), float(oWindowSize.y));
}