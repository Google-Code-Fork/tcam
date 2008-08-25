#include "TCamReader.h"

CTCamReader::CTCamReader()
{
	reset = false;
	CurrentPlayTime = 0;
	PlayUntil = 0;
}

CTCamReader::~CTCamReader()
{
}

void CTCamReader::Open(string fName)
{
	TotalPlayTime = 0;
	myRecording.open(fName.c_str(),ios::in | ios::binary | ios::ate);
	fileSize = myRecording.tellg();
	myRecording.seekg(0, ios::beg);

	data = (char*) malloc(sizeof(char)*fileSize);
	if(data == NULL)
	{
		MessageBoxA(MainDialog,"Insufficient memory or FileName not selected!","Info",MB_OK);
		MessageBoxA(MainDialog,fName.c_str(),"CamPath",MB_OK);
	}
	myRecording.read(data,fileSize);
	myRecording.close();
	
	memcpy(&TotalPlayTime,&data[fileSize-4],4);
	fileSize -= 4;
	byteOffset = 0;

	Nop(0x42330D,2);
	Nop(0x42333C,2);
}

CamPacket CTCamReader::ReadNextPacket()
{
	CamPacket ret;
	int packetSize = 0;
	int fragpacketsize = 0;
	long time = 0;

	memcpy(&ret.id,&data[byteOffset],1);
	Advance(1);
	if(ret.id == 0x30)
	{
		Sleep(100);
		memcpy((void *)XTeaAddress,&data[byteOffset],16); // Copy
		Advance(16);
		Sleep(100);
	} else if(ret.id == 0x31)
	{
		memcpy(&packetSize,&data[byteOffset],2); // Get packet header (size)
		memcpy(&ret.buf[0],&data[byteOffset],packetSize+2); // Get buffer
		Advance(packetSize+2);
		
		ret.delay = 100;
		ret.nSize = packetSize+2;
		
	} else if(ret.id == 0x33)
	{ 
		int cID = 0;
		memcpy(&cID,&data[byteOffset],4);
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
					memcpy((void *)i,&data[byteOffset],160);
					Advance(160);
					int nextPacketID = 0;
					memcpy(&nextPacketID,&data[byteOffset+4],1);

					if(nextPacketID == 0x32)
					{
						Advance(4);
					}
					break;
				}
			}
		} else 
		{
			Advance(160);
			int nextPacketID = 0;
			memcpy(&nextPacketID,&data[byteOffset+4],1);
			if(nextPacketID == 0x32)
			{
				Advance(4);
			}
		}
	} else
	{
		memcpy(&packetSize,&data[byteOffset],2); // Get packet header (size)
		Advance(2);
		memcpy(&ret.buf[0],&data[byteOffset],packetSize); // Get buffer
		Advance(packetSize);

		memcpy(&time,&data[byteOffset],4);
		Advance(4);
		ret.delay = time;
		ret.nSize = packetSize;

		CurrentPlayTime += time;
	}
	if(reset)
	{
		if(CurrentPlayTime > PlayUntil)
		{
			reset = false;
		}
	}
	return ret;
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
	reset = true;
	PlayUntil = time;
	CurrentPlayTime = 0;
}

void CTCamReader::Nop(DWORD dwAddress, int size)
{
	DWORD dwOldProtect, dwNewProtect;
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(0x42330D), size, PAGE_READWRITE, &dwOldProtect);
	memset((LPVOID)(dwAddress), 0x90, size);
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)(0x42330D), size, dwOldProtect, &dwNewProtect);
}