#include "Tibia.h"

CTibia::CTibia()
{
}

CTibia::~CTibia()
{
}

char *CTibia::CreateCharList() // Should be self explainatory
{
	unsigned char *cPacket = (unsigned char *)malloc(50); 
	ZeroMemory(&cPacket[0],50);

	cPacket[0] = 0x20;
	cPacket[1] = 0x00;
	cPacket[2] = 0x20;
	cPacket[3] = 0x00;
	cPacket[4] = 0x14;
	cPacket[5] = 0x06;
	cPacket[6] = 0x00;
	cPacket[7] = 0x33;
	cPacket[8] = 0x33;
	cPacket[9] = 0x33;
	cPacket[10] = 0x4F;
	cPacket[11] = 0x6D;
	cPacket[12] = 0x67;
	cPacket[13] = 0x64;
	cPacket[14] = 0x01;
	cPacket[15] = 5 & 0xFF;
	cPacket[16] = 0x00;
	memcpy(&cPacket[17], "Movie", 5);
	cPacket[22] = 4 & 0xFF;
	cPacket[23] = 0x00;
	memcpy(&cPacket[24], "TCam", 4);
	cPacket[28] = (char)127;
	cPacket[29] = (char)0;
	cPacket[30] = (char)0;
	cPacket[31] = (char)1;
	cPacket[32] = 0xF0; // 7152
	cPacket[33] = 0x1B;
	cPacket[34] = 0xFF;
	cPacket[35] = 0xFF;

	EncryptPacket(cPacket); // Encrypt it

	return (char *)cPacket;
}
