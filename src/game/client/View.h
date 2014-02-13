#pragma once

#include <vector>
#include <algorithm>

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
private:
	View *m_pParent;
	std::vector<View*> m_oChildren;
};