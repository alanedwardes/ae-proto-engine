#pragma once

#include "BaseGameObject.h"

class GameObjectFactoryBase
{
public:
	virtual BaseGameObject* Create() = 0;
	virtual std::string GetTypeName() = 0;
	virtual int GetTypeId() = 0;
};

template <class T>
class GameObjectFactory : public GameObjectFactoryBase
{
public:
	GameObjectFactory(const char *szTypeName, int iTypeId)
	{
		m_szTypeName = szTypeName;
		m_iTypeId = iTypeId;
	}

	std::string GetTypeName()
	{
		return m_szTypeName;
	}

	int GetTypeId()
	{
		return m_iTypeId;
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