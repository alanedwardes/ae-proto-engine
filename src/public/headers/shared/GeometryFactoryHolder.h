#pragma once

class GeometryFactoryBase;

class GeometryFactoryHolder
{
public:
	void AddGeometryFactory(GeometryFactoryBase *pEntityFactory, const char *szTypeName);
};