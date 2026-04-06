/**
 * MM CS2KZ Realtime Status
 *
 * Reports server status, player info to an API endpoint via HTTP POST.
 * CS2KZ data fields are included but left empty until cs2kz supports it.
 *
 * Configuration: addons/mm-cs2kz-rts/mm-rts.cfg
 */

#ifndef _INCLUDE_METAMOD_SOURCE_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_PLUGIN_H_

#include <ISmmPlugin.h>
#include <iserver.h>
#include "version_gen.h"

class MMSPlugin : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	void AllPluginsLoaded();

	// SourceHook callbacks
	void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick);
	void Hook_ClientPutInServer(CPlayerSlot slot, char const *pszName, int type, uint64 xuid);
	void Hook_ClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason, const char *pszName, uint64 xuid, const char *pszNetworkID);
	void Hook_OnClientConnected(CPlayerSlot slot, const char *pszName, uint64 xuid, const char *pszNetworkID, const char *pszAddress, bool bFakePlayer);
	void Hook_StartupServer(const GameSessionConfiguration_t &config, ISource2WorldSession *, const char *);
	void Hook_ServerHibernationUpdate(bool bHibernating);

public:
	const char *GetAuthor() { return PLUGIN_AUTHOR; }
	const char *GetName() { return PLUGIN_DISPLAY_NAME; }
	const char *GetDescription() { return PLUGIN_DESCRIPTION; }
	const char *GetURL() { return PLUGIN_URL; }
	const char *GetLicense() { return PLUGIN_LICENSE; }
	const char *GetVersion() { return PLUGIN_FULL_VERSION; }
	const char *GetDate() { return __DATE__; }
	const char *GetLogTag() { return PLUGIN_LOGTAG; }

private:
	void SendReport();
	void SendHibernate();

	double m_lastReportTime;
	bool m_serverActive;
};

extern MMSPlugin g_ThisPlugin;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_PLUGIN_H_
