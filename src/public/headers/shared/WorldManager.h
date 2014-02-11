#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "Manifest.h"

class BaseGameObject;
class EntityFactoryBase;
class GeometryFactoryBase;

class WorldManager
{
public:
	WorldManager();
	virtual void RemoveAllEntities();
	virtual void LoadLevel(std::string szLevelFilename);
	virtual std::vector<BaseGameObject*> GetGameObjects() const;
	virtual BaseGameObject* CreateEntity(Manifest oManifest);
	virtual BaseGameObject* CreateEntity(std::string szTypeName, Manifest oManifest = Manifest());
	virtual BaseGameObject* CreateEntity(int iTypeId, Manifest oManifest = Manifest());
	virtual bool IsLevelLoaded() { return (m_szLevelFilename.length() > 0); }
	virtual std::string LevelFilename() { return m_szLevelFilename; }
	virtual std::string LevelName();
	virtual void AddEntityFactory(EntityFactoryBase *pEntityFactory, const char *szTypeName, int iTypeId);
	virtual EntityFactoryBase* GetEntityFactory(std::string szTypeName);
	virtual EntityFactoryBase* GetEntityFactory(int iTypeId);
	virtual BaseGameObject* GetEntityById(signed int oEntityId);
private:
	virtual void AddEntity(BaseGameObject *pEntity);
	virtual BaseGameObject* CreateEntityFromFactory(EntityFactoryBase *pFactoryBase, Manifest oManifest);
	std::vector<BaseGameObject*> m_oGameObjects;
	std::string m_szLevelFilename;
	std::map<std::string, EntityFactoryBase*> m_oEntityFactoryTypeNameMap;
	std::map<int, EntityFactoryBase*> m_oEntityFactoryTypeIdMap;
	std::unique_ptr<Manifest> m_pLevelManifest;
};