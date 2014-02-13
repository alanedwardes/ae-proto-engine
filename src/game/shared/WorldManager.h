#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "IWorldManager.h"

class BaseGameObject;
class GameObjectFactoryBase;
class GeometryFactoryBase;

class WorldManager : public IWorldManager
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
	virtual BaseGameObject* GetEntityById(signed int oEntityId);
private:
	virtual void AddEntity(BaseGameObject *pEntity);
	virtual BaseGameObject* CreateEntityFromFactory(GameObjectFactoryBase *pFactoryBase, Manifest oManifest);
	std::vector<BaseGameObject*> m_oGameObjects;
	std::string m_szLevelFilename;
	std::unique_ptr<Manifest> m_pLevelManifest;
};