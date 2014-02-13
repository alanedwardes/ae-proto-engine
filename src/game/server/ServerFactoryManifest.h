#pragma once

#include "GameObjectFactory.h"
#include "ServerPhysicsEntity.h"
#include "ServerStaticEntity.h"
#include "ServerPlayerEntity.h"
#include "BaseRenderedGeometry.h"
#include "ServerStaticGeometry.h"

class ServerFactoryManifest : public IFactoryManifest
{
public:
	ServerFactoryManifest()
	{
		RegisterFactory(new GameObjectFactory<ServerPhysicsEntity>("physics_item", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<ServerStaticEntity>("static_item", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<ServerPlayerEntity>("player", m_oFactoryMap.size()));
		RegisterFactory(new GameObjectFactory<ServerStaticGeometry>("static_brush", m_oFactoryMap.size()));
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