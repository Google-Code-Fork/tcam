#include "Packet.h"

/*************************************************************************************************
Constructor
*************************************************************************************************/
CPacket::CPacket()
{
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
	
	FileHandler.WritePacket(cBuffer,16,PACKET_ID);
	

	free(cMyBuf);
}