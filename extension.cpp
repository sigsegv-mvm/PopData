#include "extension.h"
#include "helpers.h"
#include "natives.h"
#include <CDetour/detours.h>


PopData g_PopData;
SMEXT_LINK(&g_PopData);


/* globals */
CGlobalVars *gpGlobals         = nullptr;
CBaseEntityList *g_pEntityList = nullptr;

/* Source interfaces */
IServerTools *servertools = nullptr;

/* SourceMod interfaces */
IGameConfig *g_pGameConf = nullptr;

/* functions */
CWave * (CPopulationManager:: *CPopulationManager::func_GetCurrentWave)() = nullptr;

/* offsets */
int CWave::off_m_WaveSpawns               = 0x0000;
int CWaveSpawnPopulator::off_m_strName    = 0x0000;
int CWaveSpawnPopulator::off_m_ActiveBots = 0x0000;


bool PopData::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
	g_pEntityList = reinterpret_cast<CBaseEntityList *>(gamehelpers->GetGlobalEntityList());
	
	if (!gameconfs->LoadGameConfigFile("popdata", &g_pGameConf, error, maxlen)) {
		return false;
	}
	
	CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);
	
	if (g_pGameConf->GetMemSig("CPopulationManager::GetCurrentWave", reinterpret_cast<void **>(&CPopulationManager::func_GetCurrentWave))) {
		g_pSM->LogMessage(myself, "Got function: 0x%08x CPopulationManager::GetCurrentWave", *reinterpret_cast<uintptr_t *>(&CPopulationManager::func_GetCurrentWave));
	} else {
		g_pSM->LogMessage(myself, "Couldn't locate function CPopulationManager::GetCurrentWave!");
		return false;
	}
	
	if (g_pGameConf->GetOffset("CWave::m_WaveSpawns", &CWave::off_m_WaveSpawns)) {
		g_pSM->LogMessage(myself, "Got offset:   0x%08x CWave::m_WaveSpawns", CWave::off_m_WaveSpawns);
	} else {
		g_pSM->LogMessage(myself, "Couldn't get offset for CWave::m_WaveSpawns!");
		return false;
	}
	
	if (g_pGameConf->GetOffset("CWaveSpawnPopulator::m_strName", &CWaveSpawnPopulator::off_m_strName)) {
		g_pSM->LogMessage(myself, "Got offset:   0x%08x CWaveSpawnPopulator::m_strName", CWaveSpawnPopulator::off_m_strName);
	} else {
		g_pSM->LogMessage(myself, "Couldn't get offset for CWaveSpawnPopulator::m_strName!");
		return false;
	}
	
	if (g_pGameConf->GetOffset("CWaveSpawnPopulator::m_ActiveBots", &CWaveSpawnPopulator::off_m_ActiveBots)) {
		g_pSM->LogMessage(myself, "Got offset:   0x%08x CWaveSpawnPopulator::m_ActiveBots", CWaveSpawnPopulator::off_m_ActiveBots);
	} else {
		g_pSM->LogMessage(myself, "Couldn't get offset for CWaveSpawnPopulator::m_ActiveBots!");
		return false;
	}
	
	return true;
}

void PopData::SDK_OnAllLoaded()
{
	sharesys->AddNatives(myself, g_Natives);
}

void PopData::SDK_OnUnload()
{
	gameconfs->CloseGameConfigFile(g_pGameConf);
}

bool PopData::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	gpGlobals = ismm->GetCGlobals();
	
	GET_V_IFACE_CURRENT(GetServerFactory, servertools, IServerTools, VSERVERTOOLS_INTERFACE_VERSION);
	
	return true;
}
