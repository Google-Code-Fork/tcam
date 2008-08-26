#include "FileHandler.h"

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
	startClock = clock();
	delayTime = 0;
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

	Write(DELAY_ID,1);
	Write(4,2);
	Write(delayTime,4);
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

	nOffset = 0; // Reset offset
	bStreamBusy = false;
}