#pragma once

#include "IGameState.h"

class GameState : public IGameState
{
public:
	long Time() { return m_lTime; }
	void SetTime(long lTime) { m_lTime = lTime; }

	bool Running() { return m_bRunning; }
	void SetRunning(bool bRunning) { m_bRunning = bRunning; }

	Manifest *Settings() { return &m_oSettings; }

#ifdef CLIENT
	int UpdateClientId() { return m_iUpdateClientId; }
	void SetUpdateClientId(int iUpdateClientId) { m_iUpdateClientId = iUpdateClientId; }
#endif

private:
	Manifest m_oSettings;
	bool m_bRunning = true;
	long m_lTime = 0.0f;
#ifdef CLIENT
	int m_iUpdateClientId = 0;
#endif
};