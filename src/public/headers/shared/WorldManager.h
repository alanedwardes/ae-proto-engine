#pragma once

#include <vector>
#include <map>
#include <string>
#include "Manifest.h"

class BaseGameObject;
class EntityFactoryBase;
class GeometryFactoryBase;

class WorldManager
{
public:
	virtual void RemoveAllEntities();
	virtual void LoadLevel(std::string szLevelFilename);
	virtual std::vector<BaseGameObject*> GetEntities() const;
	virtual std::vector<BaseGameObject*> GetGameObjects() const;
	virtual BaseGameObject* CreateEntity(Manifest oManifest);
	virtual BaseGameObject* CreateEntity(std::string szTypeName, Manifest oManifest = Manifest());
	virtual BaseGameObject* CreateEntity(int iTypeId, Manifest oManifest = Manifest());
	virtual BaseGameObject* CreateGeometry(Manifest oManifest);
	virtual bool IsLevelLoaded() { return (m_szLevelFilename.length() > 0); }
	virtual std::string LevelFilename() { return m_szLevelFilename; }
	virtual std::string LevelName();
	virtual void AddEntityFactory(EntityFactoryBase *pEntityFactory, const char *szTypeName, int iTypeId);
	virtual void AddGeometryFactory(GeometryFactoryBase *pGeometryFactory, const char *szTypeName);
	virtual EntityFactoryBase* GetEntityFactory(std::string szTypeName);
	virtual EntityFactoryBase* GetEntityFactory(int iTypeId);
	virtual GeometryFactoryBase* GetGeometryFactory(std::string szTypeName);
	virtual BaseGameObject* GetEntityById(signed int oEntityId);
	virtual void RemoveEntityById(signed int oEntityId);
private:
	virtual void AddEntity(BaseGameObject *pEntity);
	virtual BaseGameObject* CreateEntityFromFactory(EntityFactoryBase *pFactoryBase, Manifest oManifest);
	virtual void AddGeometry(BaseGameObject *pGeometry);
	virtual BaseGameObject* CreateGeometryFromFactory(GeometryFactoryBase *pFactoryBase, Manifest oManifest);
	std::vector<BaseGameObject*> m_oEntities;
	std::vector<BaseGameObject*> m_oGeometry;
	std::vector<BaseGameObject*> m_oGameObjects;
	std::string m_szLevelFilename;
	std::map<std::string, EntityFactoryBase*> m_oEntityFactoryTypeNameMap;
	std::map<int, EntityFactoryBase*> m_oEntityFactoryTypeIdMap;
	std::map<std::string, GeometryFactoryBase*> m_oGeometryFactoryTypeNameMap;
	Manifest m_oLevelManifest;
};