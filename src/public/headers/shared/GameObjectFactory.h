#pragma once

#include "BaseGameObject.h"
#include "GameObjectFactoryHolder.h"

class EntityFactoryBase
{
public:
	virtual BaseGameObject* Create() = 0;
};

template <class T>
class GameObjectFactory : EntityFactoryBase
{
public:
	GameObjectFactory(const char *szTypeName)
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