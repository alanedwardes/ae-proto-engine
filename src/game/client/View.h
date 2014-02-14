#pragma once

#include <vector>
#include <algorithm>
#include "Point.h"

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

	virtual void Draw()
	{
		for (auto pView : m_oChildren)
			pView->Draw();
	}

	virtual void SetSize(Point poSize){ m_poSize = poSize; }
	virtual Point GetSize() { return m_poSize; }

	virtual void SetPosition(Point poPosition){ m_poPosition = poPosition; }
	virtual Point GetPosition() { return m_poPosition; }
protected:
	Point m_poSize;
	Point m_poPosition;
	View *m_pParent = nullptr;
	std::vector<View*> m_oChildren;
};

class MainView : public View
{
public:
	virtual Camera GetCamera() = 0;
};