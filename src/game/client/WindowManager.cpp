#include "WindowManager.h"
#include "Locator.h"
#include "Drawing.h"

WindowManager::WindowManager()
{
	sf::ContextSettings oContextSettings = m_oRenderWindow.getSettings();
	oContextSettings.antialiasingLevel = Locator::GameState()->Settings()->GetInt("client_antialias");

	Point poSize = Locator::GameState()->Settings()->GetPoint("client_size", Point(800, 600));

	m_oRenderWindow.create(sf::VideoMode(int(poSize.x), int(poSize.y)), "Waiting...",
		sf::Style::Default, oContextSettings);

	m_oRenderWindow.setFramerateLimit(Locator::GameState()->Settings()->GetInt("fps_limit", 60));

	auto szDebugFont = Locator::GameState()->Settings()->GetFile("debug_font");
	m_oDebugFont.loadFromFile(szDebugFont);
}

void WindowManager::AddView(View *pView)
{
	m_oViews.push_back(pView);
}

void WindowManager::Render()
{
	auto pDrawing = (Drawing*)Locator::Drawing();
	pDrawing->SetRenderTarget(&m_oRenderWindow);

	m_oRenderWindow.clear(sf::Color::White);

	for (auto pView : m_oViews)
	{
		pView->Draw();
	}

    m_oRenderWindow.display();
}