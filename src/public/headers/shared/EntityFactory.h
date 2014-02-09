#pragma once

#include "BaseGameObject.h"
#include "EntityFactoryHolder.h"

class EntityFactoryBase
{
public:
	virtual BaseGameObject* Create() = 0;
};

template <class T>
class EntityFactory : EntityFactoryBase
{
public:
	EntityFactory(const char *szTypeName)
	{
		m_szTypeName = szTypeName;
		m_iTypeId = g_oEntityFactoryHolder.AddEntityFactory(this, szTypeName);
	}

	BaseGameObject* Create()
	{
		BaseGameObject *pEntity = new T();
		pEntity->typeName = m_szTypeName;
		pEntity->typeId = m_iTypeId;
		return pEntity;
	}
private:
	std::string m_szTypeName;
	int m_iTypeId;
};