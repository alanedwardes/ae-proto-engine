#include "Client.h"
#include "ClientUpdateManager.h"
#include "WorldRenderer.h"
#include "WorldManager.h"
#include "Manifest.h"
#include "InputManager.h"
#include "ClientFactoryManifest.h"
#include "GameState.h"
#include "Locator.h"
#include "SFMLDrawing.h"
#include "WindowManager.h"
#include "UIManager.h"

void Client::Run()
{
	auto pWorldManager = new WorldManager();
	Locator::Provide(pWorldManager);

	auto pInputManager = new InputManager();
	Locator::Provide(pInputManager);

	auto pGameState = new GameState();
	Locator::Provide(pGameState);

	auto pFactoryManifest = new ClientFactoryManifest();
	Locator::Provide(pFactoryManifest);

	auto pDrawing = new SFMLDrawing();
	Locator::Provide(pDrawing);

	// Try to read the manifest
	if (!pGameState->Settings()->ReadManifest("client_manifest.json"))
		return Debug::WarningMessage("Unable to read client_manifest.json");

	// Create a network update manager
	std::string szHost = pGameState->Settings()->GetString("connect_host");
	int iPort = pGameState->Settings()->GetInt("connect_port");
	auto pClientUpdateManager = new ClientUpdateManager();
	pClientUpdateManager->SetConnection(szHost, iPort);

	// Create a window manager
	auto pWindowManager = new WindowManager();

	auto pWorldRenderer = new WorldRenderer();
	pWindowManager->AddMainView(pWorldRenderer);

	auto pUIManager = new UIManager();
	pWindowManager->AddMainView(pUIManager);

	// Log the start time
	long startTime = std::clock();

	// Send updates 30 times per second
	long lastUpdateTime = 0;
	float updateFrequency = 1000 / 30;

	// Loop while the client is running
	while (pGameState->Running())
	{
		long curTime = (std::clock() - startTime);
		pGameState->SetTime(curTime);

		// Receive updates
		pClientUpdateManager->ReceiveUpdates();

		// Process window input events
		pWindowManager->ProcessEvents();

		// Render the world
		pWindowManager->Render();

		// Send out updates
		if (curTime >= lastUpdateTime + updateFrequency)
		{
			pClientUpdateManager->SendUpdates();
			lastUpdateTime = curTime;
		}
	}
}