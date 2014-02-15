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
		if (event.type == sf::Event::MouseMoved)
		{
			for (auto pMainView : m_oMainViews)
				pMainView->MouseMoved(MouseEventToPoint(pMainView, event.mouseMove.x, event.mouseMove.y));
		}
		//auto pos = m_pWindowTarget->mapPixelToCoords(mousePos, m_oMainGameView);
		//pInputManager->NewMousePosition(POINT_FROM_SFML(pos));

        if (event.type == sf::Event::Closed)
			pGameState->SetRunning(false);

		//if (event.type == sf::Event::Resized)
		//	ResetView();

		if (event.type == sf::Event::MouseButtonPressed)
		{
			for (auto pMainView : m_oMainViews)
				pMainView->MouseDown(MouseEventToPoint(pMainView, event.mouseButton.x, event.mouseButton.y),
					pInputManager->TranslateKeyCode(event.mouseButton.button));

			pInputManager->KeyPress(
				pInputManager->TranslateKeyCode(event.mouseButton.button));
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			for (auto pMainView : m_oMainViews)
				pMainView->MouseUp(MouseEventToPoint(pMainView, event.mouseButton.x, event.mouseButton.y),
					pInputManager->TranslateKeyCode(event.mouseButton.button));

			pInputManager->KeyRelease(
				pInputManager->TranslateKeyCode(event.mouseButton.button));
		}

		if (event.type == sf::Event::MouseWheelMoved)
		{
			for (auto pMainView : m_oMainViews)
				pMainView->MouseWheel(MouseEventToPoint(pMainView, event.mouseMove.x, event.mouseMove.y),
					event.mouseWheel.delta);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			for (auto pMainView : m_oMainViews)
				pMainView->KeyDown(pInputManager->TranslateKeyCode(event.key.code));

			pInputManager->KeyPress(
				pInputManager->TranslateKeyCode(event.key.code));

			if (event.key.code == sf::Keyboard::F5)
			{
				Locator::GameState()->Settings()->SetBool("debug",
					!Locator::GameState()->Settings()->GetBool("debug"));
				Locator::GameState()->Settings()->WriteManifest();
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			for (auto pMainView : m_oMainViews)
				pMainView->KeyUp(pInputManager->TranslateKeyCode(event.key.code));

			pInputManager->KeyRelease(
				pInputManager->TranslateKeyCode(event.key.code));
		}
    }
}

Point WindowManager::MouseEventToPoint(MainView *pMainView, int x, int y)
{
	sf::Vector2i mousePos(x, y);
	auto pCamera = CameraToSFMLView(pMainView->GetCamera());
	auto poPosition = m_oRenderWindow.mapPixelToCoords(mousePos, pCamera);
	return POINT_FROM_SFML(poPosition);
}

sf::View WindowManager::CameraToSFMLView(Camera oCamera)
{
	auto oView = sf::View();
	oView.setRotation(oCamera.rotation);
	oView.zoom(oCamera.zoom);
	oView.setCenter(POINT_TO_SFML(oCamera.position));
	auto pSize = m_oRenderWindow.getSize();
	oView.setSize(pSize.x, pSize.y);
	return oView;
}

void WindowManager::Render()
{
	auto pDrawing = (SFMLDrawing*)Locator::Drawing();
	pDrawing->SetRenderTarget(&m_oRenderWindow);

	m_oRenderWindow.clear(sf::Color::White);

	for (auto pMainView : m_oMainViews)
	{
		m_oRenderWindow.setView(CameraToSFMLView(pMainView->GetCamera()));

		auto pSize = m_oRenderWindow.getSize();
		pMainView->SetSize(Point(pSize.x, pSize.y));
		pMainView->Draw();
	}

    m_oRenderWindow.display();
}