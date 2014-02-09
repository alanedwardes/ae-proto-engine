#include <algorithm>
#include <string>
#include <fstream>
#include <streambuf>
#include "GameObjectFactory.h"
#include "WorldManager.h"

#include "stlplus\file_system.hpp"

extern Manifest *g_pSettings;

void WorldManager::AddEntity(BaseGameObject *pEntity)
{
	pEntity->id = m_oGameObjects.size();
	m_oGameObjects.push_back(pEntity);
}

void WorldManager::RemoveEntityById(signed int oEntityId)
{
	auto pEntity = m_oGameObjects[oEntityId];
	m_oGameObjects[oEntityId] = nullptr;
	delete pEntity;
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

	std::string szLevelFolder = g_pSettings->GetFolder("level_directory");
	std::string szLevelPath = stlplus::create_filespec(szLevelFolder, szLevelFilename);

	if (!m_oLevelManifest.ReadManifest(szLevelPath))
		return;

	std::vector<Manifest> oEntityManifests = m_oLevelManifest.GetManifestList("entities");
	for (auto oEntityManifest : oEntityManifests)
	{
		CreateEntity(oEntityManifest);
	}

	std::vector<Manifest> oGeometryManifests = m_oLevelManifest.GetManifestList("geometry");
	for (auto oGeometryManifest : oGeometryManifests)
	{
		CreateEntity(oGeometryManifest);
	}

	m_szLevelFilename = szLevelFilename;
	std::string szLevelName = m_oLevelManifest.GetString("name", szLevelFilename);
	Debug::LogMessage("Loaded level %", szLevelName);
}

BaseGameObject* WorldManager::CreateEntity(Manifest oManifest)
{
	auto szEntityManifestType = oManifest.GetString("type");
	auto pEntityFactory = GetEntityFactory(szEntityManifestType);
	assert(pEntityFactory);
	auto pEntity = CreateEntityFromFactory(pEntityFactory, oManifest);
	return pEntity;
}

BaseGameObject* WorldManager::CreateEntity(std::string szTypeName, Manifest oManifest)
{
	auto pEntityFactory = GetEntityFactory(szTypeName);
	assert(pEntityFactory);
	return CreateEntityFromFactory(pEntityFactory, oManifest);
}

BaseGameObject* WorldManager::CreateEntity(int iTypeId, Manifest oManifest)
{
	auto pEntityFactory = GetEntityFactory(iTypeId);
	assert(pEntityFactory);
	return CreateEntityFromFactory(pEntityFactory, oManifest);
}

BaseGameObject* WorldManager::CreateEntityFromFactory(EntityFactoryBase *pFactoryBase, Manifest oManifest)
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
	return m_oLevelManifest.GetString("name", m_szLevelFilename);
}

void WorldManager::AddEntityFactory(EntityFactoryBase *pEntityFactory, const char *szTypeName, int iTypeId)
{
	m_oEntityFactoryTypeNameMap[szTypeName] = pEntityFactory;
	m_oEntityFactoryTypeIdMap[iTypeId] = pEntityFactory;
}

EntityFactoryBase* WorldManager::GetEntityFactory(std::string szTypeName)
{
	return m_oEntityFactoryTypeNameMap[szTypeName];
}

EntityFactoryBase* WorldManager::GetEntityFactory(int iTypeId)
{
	return m_oEntityFactoryTypeIdMap[iTypeId];
}