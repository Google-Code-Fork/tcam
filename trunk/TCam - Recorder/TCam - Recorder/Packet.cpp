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
	if(*PLAYER_CONNECTION >=6)
	{
	unsigned char *cMyBuf = (unsigned char *)malloc(10); // Allocate memory to hold unenecrypted packet
	memcpy(&cMyBuf[0],&cBuffer[0],10);

	cMyBuf[0] = 0x08;
	cMyBuf[1] = 0x00;

	DecryptPacket(cMyBuf); // Decrypt packet

	if(cMyBuf[4] == 0x0A) // PLAYER_ID change, only happens when relogging
	{
		FileHandler.WritePacket((unsigned char *)XTeaAddress,16,XTEA_ID);
	} 
	
	FileHandler.WritePacket(cBuffer,nSize,PACKET_ID);
	

	free(cMyBuf);
	}
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

/*************************************************************************************************
Function name: CreateContainer
Purpose: Creates container packets
Comments:
Return: Void
*************************************************************************************************/
void CPacket::CreateContainer()
{
	for(unsigned int i = CONTAINER_BEGIN; i < CONTAINER_END; i += CONTAINER_STEP)
	{
		int *isOpen = (int *)i;
		if(*isOpen == 1)
		{
			unsigned char cBuffer[492]; // Buffer
			int offset = 4;
			ZeroMemory(cBuffer,sizeof(cBuffer));

			int *contID = (int *)(i + CONTAINER_ID);
			int *contVol = (int *)(i + CONTAINER_VOL);
			int *contAmm = (int *)(i + CONTAINER_NUM_OF_ITEMS);
			
			cBuffer[offset] = 0x6E;
			offset++;
			cBuffer[offset] = (unsigned char)((i - CONTAINER_BEGIN) / CONTAINER_STEP);
			offset++;
			memcpy(&cBuffer[offset],contID,2);
			offset += 2;
			cBuffer[offset] = 0x08;
			offset++;
			cBuffer[offset] = 0x00;
			offset++;

			char *cBackPack = "Backpack";
			memcpy(&cBuffer[offset],&cBackPack[0],8);
			offset += 8;

			memcpy(&cBuffer[offset],contVol,1);
			offset++;
			cBuffer[offset] = 0x00;
			offset++;
			memcpy(&cBuffer[offset],contAmm,1);
			offset++;

			if(*contAmm > 0) 
			{
				for(int location = 0; location < *contAmm; location++)
				{
					int *id = (int *)(i + CONTAINER_ITEM_DISTANCE + (location * CONTAINER_ITEM_STEP));
					int *idData = (int *)(i + CONTAINER_ITEM_DISTANCE + (location * CONTAINER_ITEM_STEP) + 4);
					memcpy(&cBuffer[offset],id,2);
					offset += 2;
					if(DatReader.GetObjectA(*id,Stackable))
					{
						memcpy(&cBuffer[offset],idData,1);
						offset++;
					}
					if(DatReader.GetObjectA(*id,Rune))
					{
						memcpy(&cBuffer[offset],idData,1);
						offset++;
					}
					if(DatReader.GetObjectA(*id,Fluid))
					{
						memcpy(&cBuffer[offset],idData,1);
						offset++;
					}
					if(DatReader.GetObjectA(*id,Splash))
					{
						memcpy(&cBuffer[offset],idData,1);
						offset++;
					}
				}
			}

			offset -= 4;
			memcpy(&cBuffer[2],&offset,2);
			offset += 4;

			offset -= 2;
			int tempU = offset;

			while(tempU % 8 != 0)
			{
				tempU++;
			}

			memcpy(&cBuffer[0],&tempU,2);
			offset += 2;

			EncryptPacket(cBuffer);

			FileHandler.WritePacket(cBuffer,(tempU+2),PACKET_ID);
		}
	}
}
