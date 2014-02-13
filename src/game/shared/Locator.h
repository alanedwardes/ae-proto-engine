#pragma once

#include "IWorldManager.h"
#include "IWorldSimulator.h"
#include "IInputManager.h"
#include "IDrawing.h"
#include "IGameState.h"
#include "IFactoryManifest.h"

class Locator
{
public:
	// World Manager
	static IWorldManager* WorldManager();
	static void Provide(IWorldManager* pWorldManager);

	// World Simulator
	static IWorldSimulator* WorldSimulator();
	static void Provide(IWorldSimulator* pWorldSimulator);

	// Game State
	static IGameState* GameState();
	static void Provide(IGameState *pGameState);

	// Input Manager
	static IInputManager* InputManager();
	static void Provide(IInputManager *pInputManager);

	// Factory Manifest
	static IFactoryManifest* FactoryManifest();
	static void Provide(IFactoryManifest *pFactoryManifest);

	// Drawing
	static IDrawing* Drawing();
	static void Provide(IDrawing *pDrawing);
private:
	static IWorldManager *m_pWorldManager;
	static IWorldSimulator *m_pWorldSimulator;
	static IGameState *m_pGameState;
	static IInputManager *m_pInputManager;
	static IFactoryManifest *m_pFactoryManifest;
	static IDrawing *m_pDrawing;
};