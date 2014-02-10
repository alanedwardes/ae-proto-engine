#pragma once

#include "Point.h"
#include "Manifest.h"

class BaseGameObject
{
public:
	virtual void LoadManifest(Manifest oManifest);

	virtual void Init();

	virtual std::string DebugText();

	signed int id;
	std::string name;
	Point position;
	float rotation;

	int typeId;
	std::string typeName;

	bool deleted;
};