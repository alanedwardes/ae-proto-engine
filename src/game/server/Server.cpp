#include "Server.h"

#include <thread>
#include <chrono>

#include "ServerUpdateManager.h"
#include "WorldSimulator.h"
#include "shared\WorldManager.h"
#include "shared\Manifest.h"
#include "shared\GameObjectFactoryHolder.h"

// Global resources
bool g_bServerRunning = true;
Manifest *g_pSettings;
WorldManager g_oWorldManager;
WorldSimulator g_oWorldSimulator;

#include "ServerFactoryManifest.h"

void Server::Run()
{
	g_pSettings = new Manifest();

	// Try to read the manifest
	if (!g_pSettings->ReadManifest("server_manifest.json"))
		return Debug::WarningMessage("Unable to read server_manifest.json");

	// Log the start time
	long startTime = std::clock();

	// Simulate 60 times per second
	long lastSimulateTime = 0;
	float simulationFrequency = 1000.0f / 60.0f;

	// Send updates 2 times per second
	long lastUpdateTime = 0;
	float updateFrequency = 1000.0f / 25.0f;

	//// Start the server
	// Create a network update manager and listen
	int iListenPort = g_pSettings->GetInt("listen_port");
	ServerUpdateManager oUpdateManager(iListenPort);

	// Load the level defined in the manifest
	std::string szLevelName = g_pSettings->GetString("level_name");
	g_oWorldManager.LoadLevel(szLevelName);
	//// Server start (end)

	// Loop while the server is running
	while (g_bServerRunning)
	{
		long curTime = (std::clock() - startTime);

		// Receive updates
		oUpdateManager.ReceiveUpdates();

		g_oWorldSimulator.Simulate(simulationFrequency / 1000.0f);
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

	delete g_pSettings;
}