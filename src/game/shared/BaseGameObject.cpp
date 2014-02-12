#include "BaseGameObject.h"
#include <sstream>

#include "shared\GameState.h"

extern GameState g_pGameState;

std::string BaseGameObject::DebugText()
{
	std::ostringstream sstream;
	sstream << id << " " << typeName << "\n" << name;
	return sstream.str();
}

void BaseGameObject::Init()
{

}

void BaseGameObject::LoadManifest(Manifest oManifest)
{
	position = oManifest.GetPoint("position");
	rotation = oManifest.GetFloat("rotation");
	name = oManifest.GetString("name");
}