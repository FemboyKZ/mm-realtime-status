#ifndef _INCLUDE_HTTP_CLIENT_H_
#define _INCLUDE_HTTP_CLIENT_H_

#include <string>
#include <steam/steam_gameserver.h>
#include <steam/isteamhttp.h>

class HttpClient
{
public:
	HttpClient();

	bool Init();
	bool IsReady() const { return m_ready; }

	void Post(const char *url, const std::string &body, uint32 timeoutSec);

	int GetFailCount() const { return m_failCount; }
	int GetSuccessCount() const { return m_successCount; }
	void ResetCounts() { m_failCount = 0; m_successCount = 0; }

	void ReleasePending();

private:
	void OnHTTPRequestCompleted(HTTPRequestCompleted_t *pResult, bool bIOFailure);

	CSteamGameServerAPIContext m_steamAPI;
	CCallResult<HttpClient, HTTPRequestCompleted_t> m_httpCallResult;
	HTTPRequestHandle m_pendingRequest;

	bool m_ready;
	int m_failCount;
	int m_successCount;
};

extern HttpClient g_HttpClient;

#endif // _INCLUDE_HTTP_CLIENT_H_
