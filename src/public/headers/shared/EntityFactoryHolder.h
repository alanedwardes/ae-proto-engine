#pragma once

class EntityFactoryBase;

class EntityFactoryHolder
{
public:
	EntityFactoryHolder();
	int AddEntityFactory(EntityFactoryBase *pEntityFactory, const char *szTypeName);
private:
	int m_iEntityTypeIds;
};