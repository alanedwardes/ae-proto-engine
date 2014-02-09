#include <algorithm>
#include <string>
#include <fstream>
#include <streambuf>
#include "EntityFactory.h"
#include "GeometryFactory.h"
#include "WorldManager.h"

#include "stlplus\file_system.hpp"

extern Manifest *g_pSettings;

void WorldManager::AddEntity(BaseGameObject *pEntity)
{
	pEntity->id = m_oEntities.size();
	m_oEntities.push_back(pEntity);
	m_oGameObjects.push_back(pEntity);
}

void WorldManager::AddGeometry(BaseGameObject *pGeometry)
{
	m_oGeometry.push_back(pGeometry);
	m_oGameObjects.push_back(pGeometry);
}

void WorldManager::RemoveEntityById(signed int oEntityId)
{
	auto pEntity = m_oEntities[oEntityId];
	m_oEntities[oEntityId] = nullptr;
	delete pEntity;
}

BaseGameObject* WorldManager::GetEntityById(signed int oEntityId)
{
	if (oEntityId < m_oEntities.size())
		return m_oEntities[oEntityId];
	else
		return nullptr;
}

void WorldManager::RemoveAllEntities()
{
    for (auto *pEntity : m_oEntities)
        delete pEntity;

    m_oEntities.clear();
	m_oGameObjects.clear();
}

std::vector<BaseGameObject*> WorldManager::GetEntities() const
{
	return m_oEntities;
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
		CreateGeometry(oGeometryManifest);
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

BaseGameObject* WorldManager::CreateGeometry(Manifest oManifest)
{
	auto szGeometryManifestType = oManifest.GetString("type");
	auto pGeometryFactory = GetGeometryFactory(szGeometryManifestType);
	assert(pGeometryFactory);
	auto pGeometry = CreateGeometryFromFactory(pGeometryFactory, oManifest);
	return pGeometry;
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

BaseGameObject* WorldManager::CreateGeometryFromFactory(GeometryFactoryBase *pFactoryBase, Manifest oManifest)
{
	assert(pFactoryBase);

	BaseGameObject *pGeometry = pFactoryBase->Create();
	pGeometry->LoadManifest(oManifest);
	pGeometry->Init();
	AddGeometry(pGeometry);
	return pGeometry;
}

std::string WorldManager::LevelName()
{
	return m_oLevelManifest.GetString("name", m_szLevelFilename);
}

void WorldManager::AddGeometryFactory(GeometryFactoryBase *pGeometryFactory, const char *szTypeName)
{
	m_oGeometryFactoryTypeNameMap[szTypeName] = pGeometryFactory;
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

GeometryFactoryBase* WorldManager::GetGeometryFactory(std::string szTypeName)
{
	return m_oGeometryFactoryTypeNameMap[szTypeName];
}