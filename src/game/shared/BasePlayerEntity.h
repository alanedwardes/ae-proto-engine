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

	bool isCurrentPlayer;
	int playerId;
	short pressedKeys;
	Point mousePos;
};