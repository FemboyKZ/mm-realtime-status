#include <cstring>
#include <cstdio>

#include "json_builder.h"
#include "globals.h"
#include "config.h"
#include "player_manager.h"
#include "server_info.h"
#include <tier0/platform.h>
#include <tier1/convar.h>

std::string JsonEscape(const char *str)
{
	if (!str) return "";
	std::string out;
	out.reserve(strlen(str) + 16);
	for (const char *p = str; *p; p++)
	{
		switch (*p)
		{
			case '"':  out += "\\\""; break;
			case '\\': out += "\\\\"; break;
			case '\b': out += "\\b";  break;
			case '\f': out += "\\f";  break;
			case '\n': out += "\\n";  break;
			case '\r': out += "\\r";  break;
			case '\t': out += "\\t";  break;
			default:
				if (static_cast<unsigned char>(*p) < 0x20)
				{
					char buf[8];
					snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char)*p);
					out += buf;
				}
				else
				{
					out += *p;
				}
		}
	}
	return out;
}

// Resolves IP from config or hostip convar
static void ResolveIpPort(char *ip, int ipLen, int &port)
{
	if (g_Config.serverIp[0] != '\0')
	{
		strncpy(ip, g_Config.serverIp, ipLen - 1);
		ip[ipLen - 1] = '\0';
	}
	else
	{
		ConVarRefAbstract cvHostIp("hostip");
		if (cvHostIp.IsValidRef())
		{
			int hostip = cvHostIp.GetInt();
			snprintf(ip, ipLen, "%d.%d.%d.%d",
				(hostip >> 24) & 0xFF,
				(hostip >> 16) & 0xFF,
				(hostip >> 8) & 0xFF,
				hostip & 0xFF);
		}
		else
		{
			strncpy(ip, "0.0.0.0", ipLen);
		}
	}

	port = g_Config.serverPort;
	if (port <= 0)
	{
		ConVarRefAbstract cvHostPort("hostport");
		if (cvHostPort.IsValidRef())
			port = cvHostPort.GetInt();
		else
			port = 27015;
	}
}

std::string BuildPayloadJson()
{
	std::string json;
	json.reserve(4096);

	// Resolve IP and port
	char ip[64];
	int port;
	ResolveIpPort(ip, sizeof(ip), port);

	// Refresh map name each report
	g_ServerInfo.UpdateMap();

	// Player counts
	int playerCount = g_PlayerManager.GetHumanPlayerCount();
	int botCount = g_PlayerManager.GetBotCount();
	int maxPlayers = 0;
	if (g_pNetworkServerService)
	{
		INetworkGameServer *pGameServer = g_pNetworkServerService->GetIGameServer();
		if (pGameServer)
			maxPlayers = pGameServer->GetMaxClients();
	}
	if (maxPlayers <= 0 && g_pGlobals)
		maxPlayers = g_pGlobals->maxClients;

	// Build server object
	json += "{\"server\":{";
	json += "\"hostname\":\"" + JsonEscape(g_ServerInfo.hostname) + "\",";
	json += "\"os\":\"" + std::string(g_ServerInfo.osName) + "\",";
	json += "\"version\":\"" + JsonEscape(g_ServerInfo.version) + "\",";
	json += "\"tickrate\":" + std::to_string(g_ServerInfo.tickrate) + ",";
	json += std::string("\"secure\":") + (g_ServerInfo.secure ? "true" : "false") + ",";
	json += "\"mm_version\":\"" + JsonEscape(g_ServerInfo.mmVersion) + "\",";
	json += "\"cs2kz_loaded\":false,";
	json += "\"ip\":\"" + JsonEscape(ip) + "\",";
	json += "\"port\":" + std::to_string(port) + ",";
	json += "\"map\":\"" + JsonEscape(g_ServerInfo.mapName) + "\",";
	json += "\"players\":" + std::to_string(playerCount) + ",";
	json += "\"max_players\":" + std::to_string(maxPlayers) + ",";
	json += "\"bot_count\":" + std::to_string(botCount);
	json += "},";

	// Build players array
	json += "\"players\":[";
	bool firstPlayer = true;
	double now = Plat_FloatTime();

	for (int i = 0; i < MAXPLAYERS; i++)
	{
		const PlayerInfo &player = g_PlayerManager.GetPlayer(i);
		if (!player.connected || player.isBot)
			continue;

		if (!firstPlayer) json += ",";
		firstPlayer = false;

		char steamIdStr[32];
		snprintf(steamIdStr, sizeof(steamIdStr), "%llu", (unsigned long long)player.steamId64);

		double timeOnServer = 0.0;
		if (player.connectTime > 0.0)
			timeOnServer = now - player.connectTime;

		json += "{";
		json += "\"steamid\":\"" + std::string(steamIdStr) + "\",";
		json += "\"name\":\"" + JsonEscape(player.name) + "\",";
		json += "\"ip\":\"" + JsonEscape(player.ipAddress) + "\",";

		char timeBuf[32];
		snprintf(timeBuf, sizeof(timeBuf), "%.1f", timeOnServer);
		json += "\"time_on_server\":" + std::string(timeBuf) + ",";

		json += std::string("\"in_game\":") + (player.inGame ? "true" : "false") + ",";
		json += "\"cs2kz\":null";
		json += "}";
	}

	json += "]}";

	return json;
}

std::string BuildHibernateJson()
{
	char ip[64];
	int port;
	ResolveIpPort(ip, sizeof(ip), port);

	std::string json;
	json += "{\"ip\":\"" + JsonEscape(ip) + "\",\"port\":" + std::to_string(port) + "}";
	return json;
}
