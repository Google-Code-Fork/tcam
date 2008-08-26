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
	void Write(char *cBuffer, int nSize);
	void Write(int nBuffer, int nSize); 
	void Export();

	long nOffset; // Current offset in the data pointer
	unsigned char *cData; // Stores the packets as they arrive
	unsigned char *cBackBuffer; // Backbuffer used for file output
	
	ofstream myRecording; // File stream
private:
	char fileName[MAX_PATH]; // Output file path 
	bool bStreamBusy;
};

#endif