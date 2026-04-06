#ifndef _INCLUDE_SERVER_INFO_H_
#define _INCLUDE_SERVER_INFO_H_

struct ServerInfo
{
	char hostname[256];
	char mmVersion[64];
	char osName[16];
	int tickrate;
	bool secure;

	ServerInfo();
	void Cache();
};

extern ServerInfo g_ServerInfo;

#endif // _INCLUDE_SERVER_INFO_H_
