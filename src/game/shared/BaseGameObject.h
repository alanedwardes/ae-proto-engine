#pragma once

#include "Point.h"
#include "Manifest.h"

class BaseGameObject
{
public:
	virtual void LoadManifest(Manifest oManifest);

	virtual void Init();

	virtual std::string DebugText();

	signed int id = 0;
	std::string name;
	Point position;
	float rotation = 0.0f;

	int typeId = 0;
	std::string typeName;

	bool deleted = false;
};