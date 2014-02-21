#pragma once

#include "Label.h"

typedef void (MouseDownCallback)(Point poPosition, Key button); 

class Button : public Label
{
public:
	Button()
	{
		SetDrawBackground(true);
		SetBackgroundColor(Color(200, 0, 0, 255));
	}

	virtual void SetTheme(Manifest *pThemeManifest)
	{
		Label::SetTheme(pThemeManifest);

		m_oButtonManifest = pThemeManifest->GetManifest("button");
		if (!m_oButtonManifest.IsValid())
			return;
	}

	virtual void MouseOver(Point p)
	{
		SetBackgroundColor(Color(255, 0, 0, 255));
	}

	virtual void MouseOut(Point p)
	{
		SetBackgroundColor(Color(200, 0, 0, 255));
	}

	virtual void MouseDown(Point poPosition, Key button)
	{
		SetBackgroundColor(Color(100, 0, 0, 255));

		if (m_pMouseDownCallback != nullptr)
			m_pMouseDownCallback(poPosition, button);
	}

	virtual void SetMouseDownCallback(MouseDownCallback *mouseDownCallback)
	{
		m_pMouseDownCallback = mouseDownCallback;
	}

	virtual void MouseUp(Point poPosition, Key button)
	{
		SetBackgroundColor(Color(255, 0, 0, 255));
	}
private:
	MouseDownCallback *m_pMouseDownCallback = nullptr;
	Manifest m_oButtonManifest;
};