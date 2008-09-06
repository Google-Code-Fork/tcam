#include "TCamReader.h"
#include "gzip.h"

#pragma comment(lib, "zlib.lib")
using namespace zlib;
CGZip GZip;

CTCamReader::CTCamReader()
{
	bReset = false;
	nPlayUntil = 0;
}

CTCamReader::~CTCamReader()
{
}

void CTCamReader::Open(string fName)
{
	nTotalPlayTime = 0;
	char cTempSize[5];
	ZeroMemory(&cTempSize[0],sizeof(cTempSize));
	myRecording.open(fName.c_str(),ios::in | ios::binary | ios::ate);
	myRecording.seekg(0,ios::end);
	int iTemp = myRecording.tellg();
	myRecording.seekg(iTemp-4,ios::beg);
	myRecording.read(&cTempSize[0],4);
	memcpy(&fileSize,&cTempSize[0],4);
	
	data = (char*) malloc(sizeof(char)*fileSize);
	myRecording.close();

	if(data == NULL)
	{
		MessageBoxA(MainDialog,"Insufficient memory or FileName not selected!","Info",MB_OK);
	}

	if (!GZip.Open(fName.c_str(), CGZip::ArchiveModeRead))
	{
		MessageBoxA(0,"Could not open file for compression","Error",MB_OK);
	} else
	{
		GZip.ReadBufferSize((CZipper::voidp *)data,fileSize);
		GZip.Close();
	}

	byteOffset = 0;

	Nop(0x42330D,2); // Acc/Pass warning
	Nop(0x42333C,2); // Acc/Pass warning
	Nop(0x412AC2,5); // New Channel window
	Nop(0x411A7F,5); // Labels/Letters
	Nop(0x413F46,5); // Outfit window

	nCurrentPlayTime = 0;
	nTotalPlayTime = 0;
	nSpeed = 1.0;
	bFirstPSent = false;

	SendNextPacket(); // Read the header
}

void CTCamReader::SendNextPacket()
{
	SPacket NextPacket;
	NextPacket.nID = 0;
	NextPacket.nSize = 0;
	ZeroMemory(&NextPacket.cBuffer[0],sizeof(NextPacket.cBuffer));

	memcpy(&NextPacket.nID,&data[byteOffset],1); // Get ID
	Advance(1);
	memcpy(&NextPacket.nSize,&data[byteOffset],2); // Get size
	Advance(2);

	memcpy(&NextPacket.cBuffer[0],&data[byteOffset],NextPacket.nSize); // Get buffer
	Advance(NextPacket.nSize);

	switch(NextPacket.nID)
	{
	case XTEA_ID:
		{
			Sleep(100);
			memcpy((void *)XTeaAddress,&NextPacket.cBuffer[0],NextPacket.nSize); // Copy
			Sleep(100);
		}
		break;
	case BLIST_ID:
		{
			int cID = 0;
			memcpy(&cID,&NextPacket.cBuffer[0],4);
			bool inlist = false;

			for(unsigned int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i = i + BATTLELIST_STEP) {
				int *id = (int *)(i);
				if(*id == cID)
				{
					inlist = true;
				}
			}

			if(!inlist)
			{
				for(unsigned int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i = i + BATTLELIST_STEP) {
					int *id = (int *)(i);
					if(*id == 0)
					{
						memcpy((void *)i,&NextPacket.cBuffer[0],NextPacket.nSize);
						break;
					}
				}
			} 
		}
		break;
	case DELAY_ID:
		{
			unsigned int nTime = 0;
			memcpy(&nTime,&NextPacket.cBuffer[0],NextPacket.nSize);
			DelayTime(nTime);
		}
		break;
	case HEADER_ID:
		{
			memcpy(&nTotalPlayTime,&NextPacket.cBuffer[2],4);
		}
		break;
	case PACKET_ID:
		{
			NetworkClient.SendMessageClient(NextPacket.cBuffer,NextPacket.nSize);
			if(!bFirstPSent)
			{
				bFirstPSent = true;
				Sleep(500);
			}
		}
		break;
	default:
		{
			MessageBoxA(0,"Unknown packet ID during playback","Error",MB_OK);
		}
		break;
	}
}

void CTCamReader::Advance(int numBytes)
{

	if(byteOffset + numBytes > fileSize)
	{
		Sleep(3000);
		exit(1);
	}
	byteOffset += numBytes; // Increment bufferOffset
}

void CTCamReader::Reset(int time)
{
	byteOffset = 0;
	bFirstPSent = false;
	bReset = true;

	if(time <= 0)
	{
		time = 0;
		bReset = false;
		nSpeed = 1.0;
	}

	nPlayUntil = time;
	nCurrentPlayTime = 0;
}

void CTCamReader::Nop(DWORD dwAddress, int size)
{
	DWORD dwOldProtect, dwNewProtect;
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), size, PAGE_READWRITE, &dwOldProtect);
	memset((LPVOID)(dwAddress), 0x90, size);
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(dwAddress), size, dwOldProtect, &dwNewProtect);
}

void CTCamReader::DelayTime(unsigned int nMseconds)
{
	if(!bReset)
	{
		clock_t start = clock();
		while ((unsigned int)clock() < (start + nMseconds/nSpeed))
		{ 
			Sleep(1);
		}

		nCurrentPlayTime += nMseconds;

	} else
	{
		nCurrentPlayTime += nMseconds;
		if(nCurrentPlayTime > nPlayUntil)
		{
			bReset = false;
			nSpeed = 1.00;
		}
	}
}