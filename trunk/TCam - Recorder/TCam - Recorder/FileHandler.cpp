#include "FileHandler.h"
#include "gzip.h"

#pragma comment(lib, "zlib.lib")
using namespace zlib;
CGZip GZip;


/************************************************************************************************* 
Constructor
*************************************************************************************************/
CFileHandler::CFileHandler()
{
}

/************************************************************************************************* 
Destructor
*************************************************************************************************/
CFileHandler::~CFileHandler()
{
}

/*************************************************************************************************
Function name: Open
Purpose: Saves filepath for later use of file output.
Comments: Allocates roughtly 300 kb worth of memory
Return: Void
*************************************************************************************************/
void CFileHandler::Open(char *cFileName)
{
	ZeroMemory(fileName,sizeof(fileName));
	memcpy(fileName,cFileName,MAX_PATH);
	
	nOffset = 0;
	cData = (unsigned char *)malloc(300000);

	bStreamBusy = false;
	delayTime = 0;
	totalTime = 0;
	startClock = clock();
}

/*************************************************************************************************
Function name: Write
Purpose: Used for writing char arrays to buffer
Comments: Exports data once 200 kb limit is reached
Return: Void
*************************************************************************************************/
void CFileHandler::Write(unsigned char *cBuffer, int nSize)
{
	while(bStreamBusy) {} // Wait until file has been written

	if(nOffset > 204800)
	{
		Export();
	}
	memcpy(&cData[nOffset],cBuffer,nSize);
	nOffset += nSize;
}

/*************************************************************************************************
Function name: Write
Purpose: Used for writing integers to buffer
Comments: Exports data once 200 kb limit is reached
Return: Void
*************************************************************************************************/
void CFileHandler::Write(int nBuffer, int nSize)
{
	while(bStreamBusy) {} // Wait until file has been written

	if(nOffset > 204800)
	{
		Export();
	}
	memcpy(&cData[nOffset],&nBuffer,nSize);
	nOffset += nSize;
}

/*************************************************************************************************
Function name: WritePacket
Purpose: Writes a packet, defined by the protocol to the filebuffer
Comments:
Return: Void
*************************************************************************************************/
void CFileHandler::WritePacket(unsigned char *cBuffer, unsigned int nSize, unsigned int nPacketID)
{
	WriteDelay();
	Write(nPacketID,1);
	Write(nSize,2);
	Write(cBuffer,nSize);
}

/*************************************************************************************************
Function name: WriteDelay
Purpose: Outputs the delay between 2 packets
Comments:
Return: Void
*************************************************************************************************/
void CFileHandler::WriteDelay()
{
	delayTime = clock() - startClock;
	startClock = clock();
	totalTime += delayTime;

	if(*PLAYER_CONNECTION == 8)
	{
		Write(DELAY_ID,1);
		Write(4,2);
		Write(delayTime,4);
	}
}

/*************************************************************************************************
Function name: Export
Purpose: Writes to file specified by fileName variable
Comments:
Return: Void
*************************************************************************************************/
void CFileHandler::Export()
{
	bStreamBusy = true;
	myRecording.open(fileName, ios::out | ios::binary | ios::app); // Write to file
	if(!myRecording.is_open())
	{
		// Fatal error - could not get stream to open!! 
		// TODO: process with debug reporter
	}
	myRecording.write((const char *)cData,nOffset);
	myRecording.close();

	ifstream readStream;
	readStream.open(fileName, ios::in | ios::binary | ios::ate);
	readStream.seekg(0,ios_base::end);
	int length = readStream.tellg();
	unsigned char *cTemp = (unsigned char *)malloc(length);
	readStream.seekg(0,ios_base::beg);
	readStream.read((char *)cTemp,length);
	readStream.close();

	memcpy(&cTemp[5],&totalTime,4);

	// TODO: add monster kills and player kills

	
	myRecording.open(fileName, ios::out | ios::binary | ios::beg);
	myRecording.write((const char *)cTemp,length);
	myRecording.close();

	nOffset = 0; // Reset offset
	bStreamBusy = false;

	free(cTemp);
}

void CFileHandler::Compress()
{
	ifstream readStream;
	readStream.open(fileName, ios::in | ios::binary | ios::ate);
	readStream.seekg(0,ios_base::end);
	int length = readStream.tellg();
	unsigned char *cTemp = (unsigned char *)malloc(length);
	readStream.seekg(0,ios_base::beg);
	readStream.read((char *)cTemp,length);
	readStream.close();

	readStream.open(fileName, ios::trunc); // Clear the file contents
	readStream.close();

	if (!GZip.Open(fileName,CGZip::ArchiveModeWrite))
	{
		MessageBoxA(0,"Could not open file for compression","Error",MB_OK);
	} else
	{
		if(!GZip.WriteBuffer(cTemp,length))
		{
			MessageBoxA(0,"Could not write file for compression","Error",MB_OK);
		}

		GZip.Close();
	}

	char cLength[5];
	ZeroMemory(&cLength[0],sizeof(cLength));
	memcpy(&cLength[0],&length,sizeof(int));

	myRecording.open(fileName, ios::out | ios::binary | ios::app);
	myRecording.write((const char *)cLength,4);
	myRecording.close();
}