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

	int nTotalPlayTime;
	int nCurrentPlayTime;
	int nPlayUntil;

	double nSpeed;

	bool bReset;
	bool bFirstPSent;
	long byteOffset;

private:
	string fileName;
	ifstream myRecording;
	long fileSize;
	
	char *data;
	
	
};

#endif