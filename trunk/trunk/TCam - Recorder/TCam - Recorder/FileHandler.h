#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Globals.h"
#include "Constants.h"

class CFileHandler
{
public:
	CFileHandler();
	~CFileHandler();
	void Open(char *TfileName);
	bool Write(char *buf, int nSize);
	bool Write(int but, int nSize);
	void Export();

	void GenerateHeader(int mode);

	long offset;
	char *data;
	bool headerWritten;
	ofstream myRecording;
private:
	char fileName[MAX_PATH];
};

#endif