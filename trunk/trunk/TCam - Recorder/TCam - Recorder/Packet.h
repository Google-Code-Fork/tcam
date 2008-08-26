#ifndef PACKET_H
#define PACKET_H

#include "Globals.h"
#include "FileHandler.h"
#include "XTEA.h"

class CPacket: public XTEA
{
public:
	CPacket();
	~CPacket();

	void PhrasePacket(unsigned char *cBuffer, int nSize);
private:
};


#endif