#pragma once

#include "BaseRenderedGeometry.h"
#include "ISimulated.h"

class BaseSimulatedGeometry : public BaseRenderedGeometry, public ISimulated
{
public:
	virtual void LoadManifest(Manifest oManifest)
	{
		BaseRenderedGeometry::LoadManifest(oManifest);

		auto oSimulationManifests = oManifest.GetManifestList("simulate");
		for (auto oSimulationManifest : oSimulationManifests)
		{
			auto pSimulatedBody = new SimulatedBody();
			pSimulatedBody->LoadManifest(oSimulationManifest);
			m_oSimulationData.push_back(pSimulatedBody);
		}
	}

	virtual void PreSimulate()
	{
	};

	virtual void PostSimulate()
	{
	};

	virtual void Contact(ISimulated *pSimulated)
	{
	};

	virtual std::vector<SimulatedBody*> GetSimulationData()
	{
		return m_oSimulationData;
	};
protected:
	std::vector<SimulatedBody*> m_oSimulationData;
};