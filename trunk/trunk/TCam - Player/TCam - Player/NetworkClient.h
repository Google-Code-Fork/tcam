#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <winsock.h> // Winsock
#include "Globals.h"
#include "XTEA.h"

class CNetworkClient: public XTEA
{
public:
	CNetworkClient();
	~CNetworkClient();
	void Setup(int nPort, bool bLogin);
	bool StartListenClient();
	int SendMessageClient(char *lpBuffer, int nSize);
	int RecMessageClient();
	void CloseSoc();

	bool proxyReady;
	SOCKET clientSoc;

private:
	string error;
	SOCKET listenSoc;
	
};


#endif