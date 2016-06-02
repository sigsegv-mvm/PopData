#ifndef _INCLUDE_HELPERS_H_
#define _INCLUDE_HELPERS_H_


class CWaveSpawnPopulator
{
public:
	static int off_m_strName;
	const char *GetName()
	{
		/* ideally we'd craft a CUtlString pointer here, but it turns out that
		 * the crusty horrible hl2sdk-tf2 branch has an old, incompatible,
		 * not-accurate-to-what-the-game-actually-uses implementation of that
		 * class; so, we'll more-or-less reimplement SDK 2013's version of 
		 * CUtlString::Get in here */
		
		char *m_pString = *reinterpret_cast<char **>((uintptr_t)this + off_m_strName);
		
		if (m_pString != nullptr) {
			return m_pString;
		} else {
			return "";
		}
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
