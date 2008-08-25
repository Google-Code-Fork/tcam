#ifndef TCAMREADER_H
#define TCAMREADER_H

#include "Globals.h"
#include "Constants.h"
#include "XTEA.h"

class CTCamReader 
{
public:
	CTCamReader();
	~CTCamReader();
	CamPacket ReadNextPacket();
	void Open(string fName);
	void Advance(int numBytes);
	void Reset(int time);
	void Nop(DWORD dwAddress, int size);
	char bList[25000];
	int TotalPlayTime;
	int CurrentPlayTime;
	bool reset;
	long byteOffset;
private:
	string fileName;
	ifstream myRecording;
	long fileSize;
	
	char *data;
	int PlayUntil;
	
};

#endif