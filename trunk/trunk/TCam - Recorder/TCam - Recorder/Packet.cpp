#include "Packet.h"

/*************************************************************************************************
Constructor
*************************************************************************************************/
CPacket::CPacket()
{
	TibiaVersion = 822;
}

/*************************************************************************************************
Destructor
*************************************************************************************************/
CPacket::~CPacket()
{
}

/*************************************************************************************************
Function name: PhrasePacket
Purpose: Packet phraser loop, handles incoming packets
Comments:
Return: 
*************************************************************************************************/
void CPacket::PhrasePacket(unsigned char *cBuffer, int nSize)
{
	unsigned char *cMyBuf = (unsigned char *)malloc(nSize); // Allocate memory to hold unenecrypted packet
	memcpy(&cMyBuf[0],&cBuffer[0],nSize);

	DecryptPacket(cMyBuf); // Decrypt packet

	if(cMyBuf[4] == 0x0A) // PLAYER_ID change, only happens when relogging
	{
		FileHandler.WritePacket((unsigned char *)XTeaAddress,16,XTEA_ID);
	} 
	
	FileHandler.WritePacket(cBuffer,nSize,PACKET_ID);
	

	free(cMyBuf);
}

/*************************************************************************************************
Function name: CreateHeader
Purpose: Creates header info and writes to cam file
Comments: See Protocol.txt
Return: Void
*************************************************************************************************/
void CPacket::CreateHeader()
{
	unsigned char cHeader[200];
	unsigned int nTemp = 0;
	string sName = FindPlayerName(*PLAYER_ID);
	int sNameLen = strlen(sName.c_str());
	
	ZeroMemory(&cHeader[0],sizeof(cHeader)); // Clear buffer
	cHeader[0] = HEADER_ID;
	memcpy(&cHeader[3],&TibiaVersion,2); // Tibia Version
	memcpy(&cHeader[5],&nTemp,4); // Movie length
	memcpy(&cHeader[9],&sNameLen,1); // Player name length
	memcpy(&cHeader[11],sName.c_str(),sNameLen); // Player name
	memcpy(&cHeader[11 + sNameLen],&nTemp,1); // Vocation
	memcpy(&cHeader[12 + sNameLen],PLAYER_LEVEL,2);
	memcpy(&cHeader[14 + sNameLen],PLAYER_MAGIC_LEVEL,1);
	memcpy(&cHeader[15 + sNameLen],PLAYER_LEVEL,2);
	memcpy(&cHeader[17 + sNameLen],PLAYER_FIST,1);
	memcpy(&cHeader[18 + sNameLen],PLAYER_CLUB,1);
	memcpy(&cHeader[19 + sNameLen],PLAYER_SWORD,1);
	memcpy(&cHeader[20 + sNameLen],PLAYER_AXE,1);
	memcpy(&cHeader[21 + sNameLen],PLAYER_DIST,1);
	memcpy(&cHeader[22 + sNameLen],PLAYER_SHIELD,1);
	memcpy(&cHeader[23 + sNameLen],PLAYER_FISH,1);
	memcpy(&cHeader[24 + sNameLen],&nTemp,4); // Monsters seen
	memcpy(&cHeader[28 + sNameLen],&nTemp,4); // Monsters killed
	memcpy(&cHeader[32 + sNameLen],&nTemp,2); // Players seen
	memcpy(&cHeader[34 + sNameLen],&nTemp,2); // Players killed

	int tempLen = 31 + sNameLen;
	memcpy(&cHeader[1],&tempLen,2);

	FileHandler.Write(cHeader,34 + sNameLen);
}
