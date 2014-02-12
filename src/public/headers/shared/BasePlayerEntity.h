#pragma once

#include "BaseSimulatedEntity.h"

class BasePlayerEntity : public BaseSimulatedEntity
{
public:
	virtual void LoadManifest(Manifest oManifest)
	{
		oManifest.SetString("model", "player.json");

		BaseSimulatedEntity::LoadManifest(oManifest);
	};

	int playerId;
	short pressedKeys;
	Point mousePos;
};