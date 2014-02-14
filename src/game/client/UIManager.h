#pragma once

#include "View.h"
#include "Button.h"

class UIManager : public MainView
{
public:
	UIManager()
	{
		auto pButton = new Button();
		pButton->SetText("Test");
		pButton->SetPosition(Point(-100, -100));
		pButton->SetSize(Point(200, 100));
		pButton->SetTextSize(12);
		pButton->SetParent(this);
	}
	virtual Camera GetCamera() { return m_oCamera; }
private:
	Camera m_oCamera;
};