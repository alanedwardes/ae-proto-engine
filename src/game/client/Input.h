#pragma once

#include "Label.h"

class Input : public Label
{
public:
	virtual void KeyDown(Key oKey)
	{
		switch (oKey)
		{
		case KEY_BACKSPACE:
			auto szText = GetText();
			if (szText.length() > 0)
			{
				szText.pop_back();
				SetText(szText);
			}
			break;
		}
	}

	virtual void TextEntered(unsigned int iUnicodeChar)
	{
		if (iUnicodeChar > 31 && iUnicodeChar <= 255)
		{
			auto szText = GetText();
			szText += static_cast<char>(iUnicodeChar);
			SetText(szText);
		}
	}
};