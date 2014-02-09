#pragma once

#include "BaseGameObject.h"
#include "GeometryFactoryHolder.h"

class GeometryFactoryBase
{
public:
	virtual BaseGameObject* Create() = 0;
};

template <class T>
class GeometryFactory : GeometryFactoryBase
{
public:
	GeometryFactory(const char *szTypeName)
	{
		m_szTypeName = szTypeName;
		g_oGeometryFactoryHolder.AddGeometryFactory(this, szTypeName);
	}

	BaseGameObject* Create()
	{
		BaseGameObject *pEntity = new T();
		pEntity->typeName = m_szTypeName;
		return pEntity;
	}
private:
	std::string m_szTypeName;
};