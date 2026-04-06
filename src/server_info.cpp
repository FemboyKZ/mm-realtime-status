#include <cstring>

#include "server_info.h"
#include "globals.h"
#include "plugin.h"
#include <tier1/convar.h>
#include <steam/steam_gameserver.h>

ServerInfo g_ServerInfo;

ServerInfo::ServerInfo()
{
	hostname[0] = '\0';
	mmVersion[0] = '\0';
	tickrate = 64;
	secure = false;

#ifdef _WIN32
	strncpy(osName, "windows", sizeof(osName));
#else
	strncpy(osName, "linux", sizeof(osName));
#endif
}

void ServerInfo::Cache()
{
	// Hostname
	ConVarRefAbstract cvHostname("hostname");
	if (cvHostname.IsValidRef())
	{
		strncpy(hostname, cvHostname.GetString().Get(), sizeof(hostname) - 1);
		hostname[sizeof(hostname) - 1] = '\0';
	}
	else
	{
		strncpy(hostname, "unknown", sizeof(hostname));
	}

	// Tickrate from global vars
	if (g_pGlobals)
	{
		float interval = g_pGlobals->m_flIntervalPerTick;
		if (interval > 0.0f)
			tickrate = static_cast<int>(1.0f / interval + 0.5f);
	}

	// Metamod version
	int major, minor, plvers, plmin;
	g_SMAPI->GetApiVersions(major, minor, plvers, plmin);
	snprintf(mmVersion, sizeof(mmVersion), "%d.%d", major, minor);

	// VAC status
	secure = SteamGameServer_BSecure();

	META_CONPRINTF("[MM-RTS] Cached: hostname=%s, tickrate=%d, secure=%s\n",
		hostname, tickrate, secure ? "yes" : "no");
}
