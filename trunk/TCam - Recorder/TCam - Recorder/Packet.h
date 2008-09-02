#ifndef PACKET_H
#define PACKET_H

#include "Globals.h"
#include "FileHandler.h"
#include "XTEA.h"
#include "Tibia.h"
#include "DatReader.h"

class CPacket: public XTEA, public CTibia
{
public:
	CPacket();
	~CPacket();

	void PhrasePacket(unsigned char *cBuffer, int nSize);
	void CreateHeader();
	void CreateContainer();
private:
	int TibiaVersion;
};


#endif