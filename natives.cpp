#include "natives.h"
#include "helpers.h"


CPopulationManager *PopMgr()
{
	static CHandle<CPopulationManager> g_pPopulationManager = nullptr;
	
	if (g_pPopulationManager == nullptr) {
		g_pPopulationManager = reinterpret_cast<CPopulationManager *>(
			servertools->FindEntityByClassname(servertools->FirstEntity(), "info_populator"));
	}
	
	return g_pPopulationManager;
}


cell_t SP_GetWaveSpawnName(IPluginContext *pContext, const cell_t *params)
{
	if (PopMgr() == nullptr) {
		pContext->ReportError("Can't find info_populator entity!");
		return false;
	}
	
	int clientidx = params[1];
	cell_t name   = params[2];
	size_t maxlen = params[3];
	
	if (clientidx < 1 || clientidx > gpGlobals->maxClients) return false;
	
	CBaseEntity *bot = gamehelpers->ReferenceToEntity(clientidx);
	if (bot == nullptr) return false;
	
	CWave *wave = PopMgr()->GetCurrentWave();
	if (wave == nullptr) return false;
	
//	DevMsg("- Asking about bot #%d\n", clientidx);
	auto& wavespawns = wave->GetWaveSpawns();
	FOR_EACH_VEC(wavespawns, i) {
//		DevMsg("  - Looking in wavespawn %d\n", i);
		CWaveSpawnPopulator *wavespawn = wavespawns[i];
		if (wavespawn == nullptr) continue;
		
		auto& active_bots = wavespawn->GetActiveBots();
		FOR_EACH_VEC(active_bots, j) {
//			DevMsg("    - Looking at active bot %d\n", j);
			if (active_bots[j] == bot) {
//				DevMsg("      - Match!\n");
				pContext->StringToLocal(name, maxlen, wavespawn->GetName());
				return true;
			}
		}
	}
	
	return false;
}


const sp_nativeinfo_t g_Natives[] = {
	{ "PopData_GetWaveSpawnName", &SP_GetWaveSpawnName },
	{ nullptr,                    nullptr              },
};
