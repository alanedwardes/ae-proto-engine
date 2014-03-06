#pragma once

#include "View.h"
#include "Menu.h"

class UIManager : public MainView
{
public:
	UIManager()
	{
		auto pMenu = new Menu();
		pMenu->SetParent(this);
		pMenu->SetPosition(Point(40, 30));
		pMenu->SetBackgroundColor(Color(255, 255, 255, 64));
		pMenu->SetDrawBackground(true);
		pMenu->SetSize(Point(640, 480));
	}

	virtual void CalculateLayout()
	{
		MainView::CalculateLayout();

		// Make x = 0, y = 0 the top left corner
		// instead of the centre
		m_oCamera.position = GetSize() / 2;
	}

	virtual Camera GetCamera() { return m_oCamera; }
private:
	Camera m_oCamera;
};