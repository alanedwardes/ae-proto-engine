#pragma once

#include "Label.h"

class Button : public Label
{
public:
	Button()
	{
		SetDrawBackground(true);
		SetBackgroundColor(Color(200, 0, 0, 255));
	}

	virtual void MouseOver(Point p)
	{
		//Debug::DebugMessage("MouseOver, % %", p.x, p.y);
		SetBackgroundColor(Color(255, 0, 0, 255));
	}

	virtual void MouseOut(Point p)
	{
		//Debug::DebugMessage("MouseOut, % %", p.x, p.y);
		SetBackgroundColor(Color(200, 0, 0, 255));
	}

	virtual void MouseDown(Key k)
	{
		Debug::DebugMessage("mousedown");
	}
};