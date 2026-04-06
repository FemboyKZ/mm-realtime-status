#ifndef _INCLUDE_GLOBALS_H_
#define _INCLUDE_GLOBALS_H_

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <eiface.h>
#include <iserver.h>

#define MAXPLAYERS 64

extern ISource2Server *g_pSource2Server;
extern ISource2GameClients *g_pSource2GameClients;
extern IVEngineServer2 *g_pEngineServer;
extern ICvar *g_pCVar;
extern CGlobalVars *g_pGlobals;
extern INetworkServerService *g_pNetworkServerService;

#endif // _INCLUDE_GLOBALS_H_
