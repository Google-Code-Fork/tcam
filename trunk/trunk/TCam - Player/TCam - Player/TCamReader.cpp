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
	}
	myRecording.read(data,fileSize);
	myRecording.close();

	byteOffset = 0;

	Nop(0x42330D,2);
	Nop(0x42333C,2);

	CurrentPlayTime = 0;
	TotalPlayTime = 60004330;
}

void CTCamReader::SendNextPacket()
{
	SPacket NextPacket;

	memcpy(&NextPacket.nID,&data[byteOffset],1); // Get ID
	Advance(1);
	memcpy(&NextPacket.nSize,&data[byteOffset],1); // Get size
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
						memcpy((void *)i,&NextPacket.cBuffer[0],160);
						break;
					}
				}
			} 

		}
	case DELAY_ID:
		{
			unsigned int nTime = 0;
			memcpy(&nTime,&NextPacket.cBuffer[0],4);
			DelayTime(nTime);
		}
	case PACKET_ID:
		{
			NetworkClient.SendMessageClient((char *)NextPacket.cBuffer,NextPacket.nSize);
		}
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

void CTCamReader::DelayTime(unsigned int nMseconds)
{
	clock_t endwait;
	endwait = clock () + nMseconds;
	while (clock() < endwait) { Sleep(1); }
}