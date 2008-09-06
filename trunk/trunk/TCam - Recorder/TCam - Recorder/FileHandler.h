#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Globals.h"
#include "Constants.h"


class CFileHandler
{
public:
	CFileHandler();
	~CFileHandler();
	void Open(char *cFileName);
	void Write(unsigned char *cBuffer, int nSize);
	void Write(int nBuffer, int nSize); 
	void WriteDelay();
	void WritePacket(unsigned char *cBuffer, unsigned int nSize, unsigned int nPacketID);
	void Export();
	void Compress();
	
	long nOffset; // Current offset in the data pointer
	unsigned char *cData; // Stores the packets as they arrive
	
	clock_t startClock;
	unsigned int delayTime;
	unsigned int totalTime;
private:
	ofstream myRecording; // File stream
	char fileName[MAX_PATH]; // Output file path 
	bool bStreamBusy;
};

#endif