#pragma once

#include <vector>
#include <algorithm>
#include "sfml\Graphics.hpp"
#include "View.h"

class WindowManager
{
public:
	WindowManager();
	virtual void Render();
	virtual void ProcessEvents();
	virtual void AddMainView(MainView *pMainView);
private:
	virtual Point MouseEventToPoint(MainView *pMainView, int x, int y);
	virtual sf::View CameraToSFMLView(Camera oCamera);
	std::vector<MainView*> m_oMainViews;
	sf::RenderWindow m_oRenderWindow;
	sf::Image m_oWindowIcon;
	sf::Font m_oDebugFont;
};