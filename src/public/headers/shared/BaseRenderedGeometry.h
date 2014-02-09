#pragma once

#include "BaseGameObject.h"
#include "IRendered.h"

class BaseRenderedGeometry : public BaseGameObject, public IRendered
{
public:
	virtual void LoadManifest(Manifest oManifest)
	{
		BaseGameObject::LoadManifest(oManifest);

		auto oRenderManifests = oManifest.GetManifestList("render");
		for (auto oRenderManifest : oRenderManifests)
		{
			auto pRenderedPolygon = new RenderedPolygon();
			pRenderedPolygon->LoadManifest(oRenderManifest);
			m_oRenderData.push_back(pRenderedPolygon);
		}
	}

	virtual void PreSimulate()
	{
	};

	virtual void PostSimulate()
	{
	};

	virtual std::vector<RenderedPolygon*> GetRenderables()
	{
		return m_oRenderData;
	};
protected:
	std::vector<RenderedPolygon*> m_oRenderData;
};