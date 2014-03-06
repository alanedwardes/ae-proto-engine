#pragma once

#include "View.h"
#include "Input.h"
#include "Button.h"

class Menu : public View
{
public:
	Menu()
	{
		auto pButton = new Button();
		pButton->SetText("Test");
		pButton->SetPosition(Point(0, 0));
		pButton->SetSize(Point(200, 100));
		pButton->SetTextSize(20);
		pButton->SetTextColor(Color(0, 0, 0, 64));
		pButton->SetParent(this);

		pButton->SetMouseDownCallback([](Point po, Key b){
			Debug::DebugMessage("clicked");
		});

		auto pInput = new Input();
		pInput->SetText("Test");
		pInput->SetBackgroundColor(Color(0, 0, 0));
		pInput->SetDrawBackground(true);
		pInput->SetPosition(Point(0, 100));
		pInput->SetSize(Point(200, 100));
		pInput->SetTextSize(20);
		pInput->SetTextColor(Color(0, 0, 0, 64));
		pInput->SetParent(this);

		auto pTheme = new Manifest();
		pTheme->ReadManifest("client_theme.json");
		SetTheme(pTheme);
	}

	virtual void CalculateLayout()
	{
		//auto poParentSize = GetParent()->GetSize();
		//if (GetSize() != poParentSize)
		//{
		//	SetSize(poParentSize);
		//}
	}
};