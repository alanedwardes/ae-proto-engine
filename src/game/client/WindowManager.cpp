#include "WindowManager.h"
#include "Locator.h"
#include "SFMLDrawing.h"
#include "InputManager.h"
#include "GameState.h"

WindowManager::WindowManager()
{
	sf::ContextSettings oContextSettings = m_oRenderWindow.getSettings();
	oContextSettings.antialiasingLevel = Locator::GameState()->Settings()->GetInt("client_antialias");

	Point poSize = Locator::GameState()->Settings()->GetPoint("client_size", Point(800, 600));

	m_oRenderWindow.create(sf::VideoMode(int(poSize.x), int(poSize.y)), "Waiting...",
		sf::Style::Default, oContextSettings);

	m_oRenderWindow.setFramerateLimit(Locator::GameState()->Settings()->GetInt("fps_limit", 60));
}

void WindowManager::AddMainView(MainView *pMainView)
{
	m_oMainViews.push_back(pMainView);
}

void WindowManager::ProcessEvents()
{
	auto pInputManager = (InputManager*)Locator::InputManager();
	auto pGameState = (GameState*)Locator::GameState();

	sf::Event event;
    while (m_oRenderWindow.pollEvent(event))
    {
		sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
		//auto pos = m_pWindowTarget->mapPixelToCoords(mousePos, m_oMainGameView);
		//pInputManager->NewMousePosition(POINT_FROM_SFML(pos));

        if (event.type == sf::Event::Closed)
			pGameState->SetRunning(false);

		//if (event.type == sf::Event::Resized)
		//	ResetView();

		Key ePressedKey = KEY_NONE;
		if (event.type == sf::Event::KeyPressed)
		{
			pInputManager->KeyPress(
				pInputManager->TranslateKeyCode(event.key.code));

			if (event.key.code == sf::Keyboard::F5)
			{
				Locator::GameState()->Settings()->SetBool("debug",
					!Locator::GameState()->Settings()->GetBool("debug"));
				Locator::GameState()->Settings()->WriteManifest();
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			pInputManager->KeyPress(
				pInputManager->TranslateKeyCode(event.mouseButton.button));
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			pInputManager->KeyRelease(
				pInputManager->TranslateKeyCode(event.mouseButton.button));
		}

		if (event.type == sf::Event::MouseWheelMoved)
		{
			//m_flZoomLevel -= event.mouseWheel.delta / 10.0f;
		}

		if (event.type == sf::Event::KeyReleased)
		{
			pInputManager->KeyRelease(
				pInputManager->TranslateKeyCode(event.key.code));
		}
    }
}

void WindowManager::Render()
{
	auto pDrawing = (SFMLDrawing*)Locator::Drawing();
	pDrawing->SetRenderTarget(&m_oRenderWindow);

	m_oRenderWindow.clear(sf::Color::White);

	for (auto pMainView : m_oMainViews)
	{
		auto oCamera = pMainView->GetCamera();
		auto oView = sf::View();
		oView.setRotation(oCamera.rotation);
		oView.zoom(oCamera.zoom);
		oView.setCenter(POINT_TO_SFML(oCamera.position));
		auto pSize = m_oRenderWindow.getSize();
		oView.setSize(pSize.x, pSize.y);
		m_oRenderWindow.setView(oView);

		pMainView->SetSize(Point(pSize.x, pSize.y));
		pMainView->Draw();
	}

    m_oRenderWindow.display();
}