#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "GameObjectFactory.h"
#include "BaseGameObject.h"
#include "Manifest.h"

class IWorldManager
{
public:
	virtual void RemoveAllEntities() = 0;
	virtual void LoadLevel(std::string szLevelFilename) = 0;
	virtual std::vector<BaseGameObject*> GetGameObjects() const = 0;
	virtual BaseGameObject* CreateEntity(Manifest oManifest) = 0;
	virtual BaseGameObject* CreateEntity(std::string szTypeName, Manifest oManifest = Manifest()) = 0;
	virtual BaseGameObject* CreateEntity(int iTypeId, Manifest oManifest = Manifest()) = 0;
	virtual bool IsLevelLoaded() = 0;
	virtual std::string LevelFilename() = 0;
	virtual std::string LevelName() = 0;
	virtual BaseGameObject* GetEntityById(signed int oEntityId) = 0;
};