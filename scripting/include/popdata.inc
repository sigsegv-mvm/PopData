#if defined _popdata_included
#endinput
#endif
#define _popdata_included


/**
 * Get the name of the WaveSpawn that a bot was spawned from.
 * 
 * @param client        Client index.
 * @param name          String buffer in which to store the WaveSpawn name.
 * @param maxlen        Size of the string buffer.
 * @return              True if the WaveSpawn was found, false if not.
 */
native bool PopData_GetWaveSpawnName(int client, char[] name, int maxlen);
