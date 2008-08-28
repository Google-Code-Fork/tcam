#include "NetworkClient.h"
#include "Constants.h"

CNetworkClient::CNetworkClient()
{
}

CNetworkClient::~CNetworkClient()
{
}

void CNetworkClient::Setup(int nPort, bool bLogin)
{
	char LocalHost[10] = "127.0.0.1"; // Change internal IPs to localhost
	int LocalPort = nPort;
	memcpy((void *)LoginServ0,&LocalHost[0],10);
	memcpy((void *)LoginServ1,&LocalHost[0],10);
	memcpy((void *)LoginServ2,&LocalHost[0],10);
	memcpy((void *)LoginServ3,&LocalHost[0],10);
	memcpy((void *)LoginServ4,&LocalHost[0],10);
	memcpy((void *)LoginServ5,&LocalHost[0],10);
	memcpy((void *)LoginServ6,&LocalHost[0],10);
	memcpy((void *)LoginServ7,&LocalHost[0],10);
	memcpy((void *)LoginServ8,&LocalHost[0],10);
	memcpy((void *)LoginServ9,&LocalHost[0],10);

	if(bLogin)
	{
		memcpy((void *)LoginPort0,&LocalPort,2);
		memcpy((void *)LoginPort1,&LocalPort,2);
		memcpy((void *)LoginPort2,&LocalPort,2);
		memcpy((void *)LoginPort3,&LocalPort,2);
		memcpy((void *)LoginPort4,&LocalPort,2);
		memcpy((void *)LoginPort5,&LocalPort,2);
		memcpy((void *)LoginPort6,&LocalPort,2);
		memcpy((void *)LoginPort7,&LocalPort,2);
		memcpy((void *)LoginPort8,&LocalPort,2);
		memcpy((void *)LoginPort9,&LocalPort,2);
	}

	proxyReady = false;

	WSADATA wsaData;

    sockaddr_in local;

    int wsaret = WSAStartup(0x101,&wsaData); // Startup

    if(wsaret!=0)
    {
		error.append("Error: WSA failed to startup!\n");
        return;
    }

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY; // Any local address
    local.sin_port = htons((u_short)nPort); // Port 7171

    listenSoc = socket(AF_INET,SOCK_STREAM,0); // Initialize socket


    if(listenSoc == INVALID_SOCKET)
    {
		error.append("Error: Socket failed to startup!\n");
        return;
    }


    if(bind(listenSoc,(sockaddr*)&local,sizeof(local)) != 0)
    {
		error.append("Error: Failed to bind to port 7171!\n");
        return;
    }


    if(listen(listenSoc,5) != 0)
    {
		error.append("Error: Listen failed!\n");
        return;
    }

	proxyReady = true;
}


bool CNetworkClient::StartListenClient()
{

    sockaddr_in from;
    int fromlen = sizeof(from);

    clientSoc = accept(listenSoc, (struct sockaddr*)&from, &fromlen); // Accept connection

	if(clientSoc != INVALID_SOCKET)
	{
		return true;
	}
	
	error.append("Error: Accept failed!\n");
	return false;
}

int CNetworkClient::SendMessageClient(char *lpBuffer, int nSize)
{
	return send(clientSoc,lpBuffer,nSize,0);
}

int CNetworkClient::RecMessageClient()
{
	char buffer[10000];
	int RecievedBytes;
	ZeroMemory(buffer,sizeof(buffer));

	RecievedBytes = recv(clientSoc,buffer,10000,0);

	if(RecievedBytes == SOCKET_ERROR)
	{
		return 0;
	}
	else
	{
		DecryptPacket((unsigned char *)buffer);

		int MessageLen = 0;
		int PacketID = 0;

		memcpy(&PacketID,&buffer[4],1);
		if(PacketID == 0x96)
		{
			memcpy(&MessageLen,&buffer[6],1);
			if(MessageLen == 9)
			{

			char Command[3];
			ZeroMemory(Command,sizeof(Command));
			memcpy(&Command[0],&buffer[8],3);

			char cHour[2];
			char cMinute[2];
			memcpy(&cHour[0],&buffer[12],2);
			memcpy(&cMinute[0],&buffer[15],2);

			int Hour = atoi(cHour);
			int Minute = atoi(cMinute);

			if(strcmp(Command,"ffu") == 0)
			{
				return (((Hour * 3600) + (Minute * 60))*1000);
			}
			}
		}
		return 0;
	}
	return 0;
}

void CNetworkClient::CloseSoc()
{
	shutdown(clientSoc,2);

}