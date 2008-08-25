#ifndef PACKET_H
#define PACKET_H

#include "Globals.h"
#include "Tibia.h"
#include "Constants.h"
#include "DatReader.h"
#include "XTEA.h"
#include "FileHandler.h"

class CPacket: public CTibia, public XTEA
{
public:
	CPacket();
	~CPacket();

	bool Allocate(unsigned int memCount);
	bool Flush();

	void Write(char *cBuffer, int nSize);
	void Write(int iBuffer, int nSize);

	void Advance(unsigned int nSize);
	void GenerateHeader(bool hEncrypted);

	bool MapPacketBelow();
	bool MapPacketAbove();

	bool Battlelist();
private:
	unsigned char *data;
	unsigned int offset;
};

#endif