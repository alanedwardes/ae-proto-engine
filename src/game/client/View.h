#pragma once

#include <vector>
#include <algorithm>
#include "Point.h"
#include "Key.h"
#include "Color.h"
#include "Locator.h"

struct Camera
{
	Point position;
	float zoom = 0.0f;
	float rotation = 0.0f;
};

class View
{
public:
	virtual void SetParent(View *pParent)
	{
		// If the parent was set previously,
		// remove ourself from its children
		if (m_pParent != nullptr)
			m_pParent->RemoveChild(this);

		// Set new parent
		m_pParent = pParent;

		// Add ourself to the new parent
		pParent->AddChild(this);
	}

	virtual void AddChild(View *pChild)
	{
		auto oPosition = std::find(m_oChildren.begin(), m_oChildren.end(), pChild);
		if (oPosition == m_oChildren.end())
			m_oChildren.push_back(pChild);
	}

	virtual void RemoveChild(View *pChild)
	{
		auto oPosition = std::find(m_oChildren.begin(), m_oChildren.end(), pChild);
		if (oPosition != m_oChildren.end())
			m_oChildren.erase(oPosition);
	}

	virtual void MouseOver(Point poPosition)
	{
		for (auto pView : m_oChildren) pView->MouseOver(poPosition);
	}

	virtual void MouseOut(Point poPosition)
	{
		for (auto pView : m_oChildren) pView->MouseOut(poPosition);
	}

	virtual void MouseMoved(Point poPosition)
	{
		for (auto pView : m_oChildren)
		{
			if (pView->IsPointInside(poPosition))
				MouseOver(poPosition);
			else
				MouseOut(poPosition);
			pView->MouseMoved(poPosition);
		}
	}

	virtual void MouseDown(Point poPosition, Key button)
	{
		for (auto pView : m_oChildren)
			if (pView->IsPointInside(poPosition))
				pView->MouseDown(poPosition, button);
	}

	virtual void MouseUp(Point poPosition, Key button)
	{
		for (auto pView : m_oChildren)
			if (pView->IsPointInside(poPosition))
				pView->MouseUp(poPosition, button);
	}

	virtual void MouseWheel(Point poPosition, int iDelta)
	{
		for (auto pView : m_oChildren)
			if (pView->IsPointInside(poPosition))
				pView->MouseWheel(poPosition, iDelta);
	}

	virtual void SetTheme(Manifest *pThemeManifest)
	{
		auto oViewManifest = pThemeManifest->GetManifest("view");
		if (oViewManifest.IsValid())
		{
			
		}

		for (auto pView : m_oChildren) pView->SetTheme(pThemeManifest);
	}

	virtual void TextEntered(unsigned int iUnicodeChar)
	{
		for (auto pView : m_oChildren) pView->TextEntered(iUnicodeChar);
	}

	virtual void KeyDown(Key oKey)
	{
		for (auto pView : m_oChildren) pView->KeyDown(oKey);
	}

	virtual void KeyUp(Key oKey)
	{
		for (auto pView : m_oChildren) pView->KeyUp(oKey);
	}

	virtual void Draw()
	{
		if (m_bDrawBackgroundColor)
		{
			Locator::Drawing()->SetColor(m_coBackgroundColor);
			Locator::Drawing()->DrawRectangle(m_poSize, m_poPosition);
		}

		for (auto pView : m_oChildren) pView->Draw();
	}

	virtual void CalculateLayout()
	{
		for (auto pView : m_oChildren) pView->CalculateLayout();
	}

	virtual void SetBackgroundColor(Color coColor)
	{
		m_coBackgroundColor = coColor;
	}

	virtual void SetDrawBackground(bool bDraw)
	{
		m_bDrawBackgroundColor = bDraw;
	}

	virtual bool GetDrawBackground()
	{
		return m_bDrawBackgroundColor;
	}

	virtual Color GetBackgroundColor()
	{
		return m_coBackgroundColor;
	}

	bool IsPointInside(Point poPoint)
	{
		auto poLower = GetPosition();
		auto poUpper = GetPosition() + GetSize();

		if (poPoint.x > poLower.x && poPoint.y > poLower.y)
		{
			if (poPoint.x < poUpper.x && poPoint.y < poUpper.y)
			{
				return true;
			}
		}

		return false;
	}

	virtual void SetSize(Point poSize)
	{
		m_poSize = poSize;
		CalculateLayout();
	}

	virtual Point GetSize() { return m_poSize; }

	virtual void SetPosition(Point poPosition)
	{
		m_poPosition = poPosition;
		CalculateLayout();
	}
	virtual Point GetPosition() { return m_poPosition; }
private:
	Point m_poSize;
	Point m_poPosition;
	Color m_coBackgroundColor;
	bool m_bDrawBackgroundColor = false;
	View *m_pParent = nullptr;
	std::vector<View*> m_oChildren;
};

class MainView : public View
{
public:
	virtual Camera GetCamera() = 0;
};