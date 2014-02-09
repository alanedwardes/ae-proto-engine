#include "Client.h"
#include "ClientUpdateManager.h"
#include "WorldRenderer.h"
#include "shared\WorldManager.h"
#include "shared\Manifest.h"
#include "shared\GameObjectFactoryHolder.h"
#include "InputManager.h"

// Global resources
bool g_bClientRunning = true;
Manifest *g_pSettings;
WorldManager g_oWorldManager;
ClientUpdateManager g_oClientUpdateManager;
InputManager *g_pInputManager;

#include "ClientFactoryManifest.h"

void Client::Run()
{
	g_pSettings = new Manifest();
	g_pInputManager = new InputManager();

	// Try to read the manifest
	if (!g_pSettings->ReadManifest("client_manifest.json"))
		return Debug::WarningMessage("Unable to read client_manifest.json");

	// Create a network update manager
	std::string szHost = g_pSettings->GetString("connect_host");
	int iPort = g_pSettings->GetInt("connect_port");
	g_oClientUpdateManager.SetConnection(szHost, iPort);

	// Create a world renderer
	WorldRenderer oWorldRenderer;

	// Log the start time
	long startTime = std::clock();

	// Send updates 20 times per second
	long lastUpdateTime = 0;
	float updateFrequency = 1000 / 20;

	// Loop while the client is running
	while (g_bClientRunning)
	{
		long curTime = (std::clock() - startTime);

		// Receive updates
		g_oClientUpdateManager.ReceiveUpdates();

		// Process window input events
		oWorldRenderer.ProcessEvents();

		// Render the world
		oWorldRenderer.Render();

		// Send out updates
		if (curTime >= lastUpdateTime + updateFrequency)
		{
			g_oClientUpdateManager.SendUpdates();
			lastUpdateTime = curTime;
		}
	}

	delete g_pSettings;
	delete g_pInputManager;
}