#include "GameObjectFactoryHolder.h"
#include "WorldManager.h"

extern WorldManager g_oWorldManager;

EntityFactoryHolder::EntityFactoryHolder()
{
	m_iEntityTypeIds = 0;
}

int EntityFactoryHolder::AddEntityFactory(EntityFactoryBase *pEntityFactory, const char *szTypeName)
{
	m_iEntityTypeIds++;

	g_oWorldManager.AddEntityFactory(pEntityFactory, szTypeName, m_iEntityTypeIds);

	return m_iEntityTypeIds;
}