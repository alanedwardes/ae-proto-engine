#pragma once

class IFactoryManifest
{
public:
	virtual GameObjectFactoryBase* GetFactory(std::string szFactoryType) = 0;
	virtual GameObjectFactoryBase* GetFactory(int szFactoryTypeId) = 0;
private:
	virtual void RegisterFactory(GameObjectFactoryBase *pFactory) = 0;
};