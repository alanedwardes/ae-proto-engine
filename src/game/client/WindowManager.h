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
private:
	sf::RenderWindow m_oRenderWindow;
	sf::Image m_oWindowIcon;
	sf::Font m_oDebugFont;
};