#pragma once

#include "BaseGameObject.h"
#include "Manifest.h"
#include "IRendered.h"
#include "ISimulated.h"
#include "StreamSerialiser.h"

extern Manifest *g_pSettings;

class BaseSimulatedEntity : public BaseGameObject, public IRendered, public ISimulated
{
public:
	virtual void LoadManifest(Manifest oManifest)
	{
		BaseGameObject::LoadManifest(oManifest);

		auto oModelManifest = oManifest.GetIncludedManifest("model",
			g_pSettings->GetFolder("model_directory"));

		auto oSimulationManifests = oModelManifest.GetManifestList("simulate");
		for (auto oSimulationManifest : oSimulationManifests)
		{
			auto pSimulatedBody = new SimulatedBody();
			pSimulatedBody->LoadManifest(oSimulationManifest);
			m_oSimulationData.push_back(pSimulatedBody);
		}

		auto oRenderManifests = oModelManifest.GetManifestList("render");
		for (auto oRenderManifest : oRenderManifests)
		{
			auto pRenderedPolygon = new RenderedPolygon();
			pRenderedPolygon->LoadManifest(oRenderManifest);
			m_oRenderData.push_back(pRenderedPolygon);
		}
	};

	virtual void PreSimulate()
	{
	};

	virtual void PostSimulate()
	{
	};

	virtual std::vector<SimulatedBody*> GetSimulationData()
	{
		return m_oSimulationData;
	};

	virtual std::vector<RenderedPolygon*> GetRenderables()
	{
		return m_oRenderData;
	};
protected:
	std::vector<SimulatedBody*> m_oSimulationData;
	std::vector<RenderedPolygon*> m_oRenderData;
};