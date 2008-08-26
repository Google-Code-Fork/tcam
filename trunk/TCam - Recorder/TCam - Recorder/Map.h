#ifndef MAP_H
#define MAP_H

#include "Globals.h"
#include "Tibia.h"
#include "Constants.h"
#include "DatReader.h"
#include "XTEA.h"
#include "FileHandler.h"

class CMap: public CTibia, public XTEA
{
public:
	CMap();
	~CMap();

	bool Allocate(unsigned int memCount);
	void Flush();

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