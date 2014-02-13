#pragma once

#include "GameObjectFactory.h"

#include "BaseSimulatedEntity.h"
#include "ClientPhysicsEntity.h"
#include "ClientPlayerEntity.h"

#include "BaseSimulatedGeometry.h"
#include "BaseRenderedGeometry.h"

#include "IFactoryManifest.h"

class ClientFactoryManifest : public IFactoryManifest
{
public:
	ClientFactoryManifest()
	{
		RegisterFactory(new GameObjectFactory<ClientPhysicsEntity>("physics_item", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<BaseSimulatedEntity>("static_item", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<ClientPlayerEntity>("player", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<BaseSimulatedGeometry>("static_brush", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<BaseRenderedGeometry>("detail_brush", m_oFactoryMap.size()));
	};

	virtual GameObjectFactoryBase* GetFactory(std::string szFactoryType)
	{
		for (auto pFactory : m_oFactoryMap)
			if (pFactory->GetTypeName() == szFactoryType)
				return pFactory;

		return nullptr;
	}

	virtual GameObjectFactoryBase* GetFactory(int szFactoryTypeId)
	{
		return m_oFactoryMap[szFactoryTypeId];
	}
private:
	virtual void RegisterFactory(GameObjectFactoryBase *pFactory)
	{
		m_oFactoryMap.push_back(pFactory);
	};
	std::vector<GameObjectFactoryBase*> m_oFactoryMap;
};