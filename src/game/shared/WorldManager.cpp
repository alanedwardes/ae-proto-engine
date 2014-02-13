#include <algorithm>
#include <string>
#include <fstream>
#include <streambuf>
#include "GameObjectFactory.h"
#include "WorldManager.h"
#include "Locator.h"

#include "stlplus\file_system.hpp"

WorldManager::WorldManager()
{
	m_pLevelManifest.reset(new Manifest());
}

void WorldManager::AddEntity(BaseGameObject *pEntity)
{
	pEntity->id = m_oGameObjects.size();
	m_oGameObjects.push_back(pEntity);
}

BaseGameObject* WorldManager::GetEntityById(signed int oEntityId)
{
	if (oEntityId < m_oGameObjects.size())
		return m_oGameObjects[oEntityId];
	else
		return nullptr;
}

void WorldManager::RemoveAllEntities()
{
    for (auto *pEntity : m_oGameObjects)
        delete pEntity;

    m_oGameObjects.clear();
}

std::vector<BaseGameObject*> WorldManager::GetGameObjects() const
{
	return m_oGameObjects;
}

void WorldManager::LoadLevel(std::string szLevelFilename)
{
	if (IsLevelLoaded())
		return;

	RemoveAllEntities();
	m_szLevelFilename = "";

	std::string szLevelFolder = Locator::GameState()->Settings()->GetFolder("level_directory");
	std::string szLevelPath = stlplus::create_filespec(szLevelFolder, szLevelFilename);

	if (!m_pLevelManifest->ReadManifest(szLevelPath))
		return;

	std::vector<Manifest> oEntityManifests = m_pLevelManifest->GetManifestList("entities");
	for (auto oEntityManifest : oEntityManifests)
	{
		CreateEntity(oEntityManifest);
	}

	std::vector<Manifest> oGeometryManifests = m_pLevelManifest->GetManifestList("geometry");
	for (auto oGeometryManifest : oGeometryManifests)
	{
		CreateEntity(oGeometryManifest);
	}

	m_szLevelFilename = szLevelFilename;
	std::string szLevelName = m_pLevelManifest->GetString("name", szLevelFilename);
	Debug::LogMessage("Loaded level %", szLevelName);
}

BaseGameObject* WorldManager::CreateEntity(Manifest oManifest)
{
	auto szEntityManifestType = oManifest.GetString("type");
	auto pGameObjectFactory = Locator::FactoryManifest()->GetFactory(szEntityManifestType);
	assert(pGameObjectFactory);
	auto pEntity = CreateEntityFromFactory(pGameObjectFactory, oManifest);
	return pEntity;
}

BaseGameObject* WorldManager::CreateEntity(std::string szTypeName, Manifest oManifest)
{
	auto pGameObjectFactory = Locator::FactoryManifest()->GetFactory(szTypeName);
	assert(pGameObjectFactory);
	return CreateEntityFromFactory(pGameObjectFactory, oManifest);
}

BaseGameObject* WorldManager::CreateEntity(int iTypeId, Manifest oManifest)
{
	auto pGameObjectFactory = Locator::FactoryManifest()->GetFactory(iTypeId);
	assert(pGameObjectFactory);
	return CreateEntityFromFactory(pGameObjectFactory, oManifest);
}

BaseGameObject* WorldManager::CreateEntityFromFactory(GameObjectFactoryBase *pFactoryBase, Manifest oManifest)
{
	if (pFactoryBase == nullptr)
	{
		Debug::WarningMessage("Entity with no factory!");
		return nullptr;
	}

	BaseGameObject *pEntity = pFactoryBase->Create();
	pEntity->LoadManifest(oManifest);
	pEntity->Init();
	AddEntity(pEntity);
	return pEntity;
}

std::string WorldManager::LevelName()
{
	return m_pLevelManifest->GetString("name", m_szLevelFilename);
}