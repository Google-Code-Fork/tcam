#ifndef TCAMREADER_H
#define TCAMREADER_H

#include "Globals.h"
#include "Constants.h"
#include "XTEA.h"
#include "NetworkClient.h"

class CTCamReader 
{
public:
	CTCamReader();
	~CTCamReader();
	
	void Open(string fName);
	void Advance(int numBytes);
	void Reset(int time);
	void Nop(DWORD dwAddress, int size);
	void SendNextPacket();
	void DelayTime(unsigned int nMseconds);

	int TotalPlayTime;
	int CurrentPlayTime;
	bool reset;
	long byteOffset;
	bool bFirstPSent;
private:
	string fileName;
	ifstream myRecording;
	long fileSize;
	
	char *data;
	int PlayUntil;
	
};

#endif