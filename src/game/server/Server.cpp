#include "Server.h"

#include <thread>
#include <chrono>

#include "ServerUpdateManager.h"
#include "WorldSimulator.h"
#include "WorldManager.h"
#include "Manifest.h"
#include "GameState.h"
#include "ServerFactoryManifest.h"
#include "Locator.h"

void Server::Run()
{
	auto pWorldManager = new WorldManager();
	Locator::Provide(pWorldManager);

	auto pWorldSimulator = new WorldSimulator;
	Locator::Provide(pWorldSimulator);

	auto pGameState = new GameState();
	Locator::Provide(pGameState);

	auto pFactoryManifest = new ServerFactoryManifest();
	Locator::Provide(pFactoryManifest);

	// Try to read the manifest
	if (!pGameState->Settings()->ReadManifest("server_manifest.json"))
		return Debug::WarningMessage("Unable to read server_manifest.json");

	// Log the start time
	long startTime = std::clock();

	// Simulate 60 times per second
	long lastSimulateTime = 0;
	float simulationFrequency = 1000.0f / pGameState->Settings()->GetFloat("update_rate", 60.0f);

	// Send updates 30 times per second
	long lastUpdateTime = 0;
	float updateFrequency = 1000.0f / pGameState->Settings()->GetFloat("simulation_rate", 30.0f);

	//// Start the server
	// Create a network update manager and listen
	int iListenPort = pGameState->Settings()->GetInt("listen_port");
	ServerUpdateManager oUpdateManager(iListenPort);

	// Load the level defined in the manifest
	std::string szLevelName = pGameState->Settings()->GetString("level_name");
	pWorldManager->LoadLevel(szLevelName);
	//// Server start (end)

	// Loop while the server is running
	while (pGameState->Running())
	{
		long curTime = (std::clock() - startTime);
		pGameState->SetTime(curTime);

		// Receive updates
		oUpdateManager.ReceiveUpdates();

		pWorldSimulator->Simulate(simulationFrequency / 1000.0f);
		lastSimulateTime = curTime;

		// Send out updates
		if (curTime >= lastUpdateTime + updateFrequency)
		{
			oUpdateManager.SendUpdates();
			lastUpdateTime = curTime;
		}

		std::chrono::milliseconds dura( 16 );
		std::this_thread::sleep_for( dura );
	}
}