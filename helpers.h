#ifndef _INCLUDE_HELPERS_H_
#define _INCLUDE_HELPERS_H_


class CWaveSpawnPopulator
{
public:
	static int off_m_strName;
	CUtlString& GetName()
	{
		return *reinterpret_cast<CUtlString *>((uintptr_t)this + off_m_strName);
	}
	
	static int off_m_ActiveBots;
	CUtlVector<CHandle<CBaseEntity>>& GetActiveBots()
	{
		return *reinterpret_cast<CUtlVector<CHandle<CBaseEntity>> *>((uintptr_t)this + off_m_ActiveBots);
	}
};


class CWave
{
public:
	static int off_m_WaveSpawns;
	CUtlVector<CWaveSpawnPopulator *>& GetWaveSpawns()
	{
		return *reinterpret_cast<CUtlVector<CWaveSpawnPopulator *> *>((uintptr_t)this + off_m_WaveSpawns);
	}
};


class CPopulationManager
{
public:
	static CWave * (CPopulationManager::*func_GetCurrentWave)();
	CWave *GetCurrentWave()
	{
		return (this->*func_GetCurrentWave)();
	}
};


// TODO: attempt to do some sanity checks on our struct offsets at runtime


#endif // _INCLUDE_HELPERS_H_
