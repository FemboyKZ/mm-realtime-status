#ifndef _INCLUDE_CONFIG_H_
#define _INCLUDE_CONFIG_H_

struct PluginConfig
{
	char apiUrl[256];
	char apiKey[256];
	char serverIp[64];
	int serverPort;
	float interval;

	PluginConfig();
	void Load();
};

extern PluginConfig g_Config;

#endif // _INCLUDE_CONFIG_H_
