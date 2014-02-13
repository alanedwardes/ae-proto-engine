#pragma once

#include "Manifest.h"

class IGameState
{
public:
	virtual long Time() = 0;
	virtual bool Running() = 0;
	virtual Manifest* Settings() = 0;
#ifdef CLIENT
	virtual int UpdateClientId() = 0;
#endif
};