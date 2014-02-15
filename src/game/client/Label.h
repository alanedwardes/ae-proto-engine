#pragma once

#include "View.h"

class Label : public View
{
public:
	virtual void Draw()
	{
		View::Draw();

		Locator::Drawing()->SetColor(Color(0, 0, 0, 255));
		Locator::Drawing()->DrawText(m_szLabelText, 0, m_iLabelTextSize, GetPosition());
	}

	virtual void SetText(std::string szLabelText){ m_szLabelText = szLabelText; }
	virtual std::string GetText() { return m_szLabelText; }

	virtual void SetTextSize(int iLabelTextSize) { m_iLabelTextSize = iLabelTextSize; }
	virtual int GetTextSize() { return m_iLabelTextSize; }
protected:
	int m_iLabelTextSize = 0;
	std::string m_szLabelText;
};