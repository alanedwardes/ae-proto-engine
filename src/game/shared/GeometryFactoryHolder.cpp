#include "GeometryFactoryHolder.h"
#include "WorldManager.h"

extern WorldManager g_oWorldManager;

void GeometryFactoryHolder::AddGeometryFactory(GeometryFactoryBase *pEntityFactory, const char *szTypeName)
{
	g_oWorldManager.AddGeometryFactory(pEntityFactory, szTypeName);
}