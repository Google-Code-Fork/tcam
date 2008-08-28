#include "Map.h"

CDatReader DatReader("Tibia.dat");

/*************************************************************************************************
Constructor
*************************************************************************************************/
CMap::CMap()
{
}

/*************************************************************************************************
Destructor
*************************************************************************************************/
CMap::~CMap()
{
}

/*************************************************************************************************
Function name: Allocate
Purpose: Creates memory for storage of map
Comments:
Return: True if data is allocated, false otherwise
*************************************************************************************************/
bool CMap::Allocate(unsigned int memCount)
{
	data = (unsigned char *)malloc(memCount);
	offset = 4;

	if(data != NULL)
	{
		ZeroMemory(data,memCount);
		return true;
	}

	return false;
}

/*************************************************************************************************
Function name: Flush
Purpose: Frees the data allocated for the map and resets the offset
Comments:
Return: Void
*************************************************************************************************/
void CMap::Flush()
{
	free(data);
	offset = 4;
}

/*************************************************************************************************
Function name: Advance
Purpose: Increments the offset pointer
Comments:
Return: Void
*************************************************************************************************/
void CMap::Advance(unsigned int nSize)
{
	offset += nSize;
}

/*************************************************************************************************
Function name: Write
Purpose: Writes char arrays to buffer
Comments:
Return: Void
*************************************************************************************************/
void CMap::Write(char *cBuffer, int nSize)
{
	memcpy(&data[offset],cBuffer,nSize);
	offset += nSize;
}

/*************************************************************************************************
Function name: Write
Purpose: Writes integers to buffer
Comments:
Return: Void
*************************************************************************************************/
void CMap::Write(int iBuffer, int nSize)
{
	memcpy(&data[offset],&iBuffer,nSize);
	offset += nSize;
}

/*************************************************************************************************
Function name: GenerateHeader
Purpose: Generates the packet header
Comments: Packet header must be a value divisible by 8 for XTEA encryption
		  hEncrypted parameter specifies if the function is to make the unenecypted lenth as well
Return: Void
*************************************************************************************************/
void CMap::GenerateHeader(bool hEncrypted)
{
	if(hEncrypted)
	{
		offset -= 4;
		int temp = offset;
		memcpy(&data[2],&temp,2);
		offset += 4;
	}

	offset -= 2;
	int tempU = offset;

	while(tempU % 8 != 0)
	{
		tempU++;
	}

	memcpy(&data[0],&tempU,2);

	offset += 2;
}

/*************************************************************************************************
Function name: MapPacketBelow
Purpose: Generates the mappacket below ground level
Comments: 
Return: True if succeeded
*************************************************************************************************/
bool CMap::MapPacketBelow()
{
	Allocate(50000);

	Write(0x0A,1);
	Write(*PLAYER_ID,4);
	Write(0x32,1);
	Write(0x00,1);
	Write(0x00,1);
	Write(0x64,1);
	Write(BattleListGet(*PLAYER_ID,BATTLELIST_X),2);
	Write(BattleListGet(*PLAYER_ID,BATTLELIST_Y),2);
	Write(BattleListGet(*PLAYER_ID,BATTLELIST_Z),1);

	for(int nz = (*PLAYER_Z - 2); nz <= (*PLAYER_Z + 2); nz++)
	{
		for(int nx = 0; nx < 18; nx++)
		{
			for(int ny = 0; ny < 14; ny++)
			{
				if(nz < 16)
				{
					int playerTileNum = GetPlayerTileNum();
					int myZ = playerTileNum / (14 * 18);
					int myY = (playerTileNum - myZ * 14 * 18) / 18;
					int myX = (playerTileNum - myZ * 14 * 18) - myY * 18;

					if(playerTileNum < 0)
					{
						return false;
					}
					//Credits to Stiju for the tempx,tempy code sniplets :D
					int tempx = 17 - ((17 - (nx + myX - 8)) % 18);
					tempx = (tempx <= 18)? tempx : -((17 - (nx + myX - 8)) % 18) - 1;
					int tempy = 13 - ((13 - (ny + myY - 6)) % 14);
					tempy = (tempy <= 14)? tempy : -((13 - (ny + myY - 6)) % 14) - 1;

					int tempnZ = myZ + (*PLAYER_Z - nz);
					if(tempnZ > 7)
					{
						tempnZ = tempnZ - 8;
					}

					if(tempx % 18 == 0)
						tempx = 0;
					if(tempy % 14 == 0)
						tempy = 0;

					STile tile;
					memcpy(&tile,(void *)(*MAP_POINTER + (tempnZ * 43344) + (tempx * 172) + (tempy * 3096)),sizeof(tile));


					if(tile.Count > 0)
					{
						if(tile.Count > 13)
						{
							return false;
						}
						for(unsigned int ns = 0; ns < tile.Count; ns++)
						{

							if(tile.TileObject[ns].ObjectID == 99)
							{
								Write(0x61,1);
								Write(0x00,1);
								Write(0x00,1);
								Write(0x00,1);
								Write(0x00,1);
								Write(0x00,1);

								Write(tile.TileObject[ns].Data1,4);
								string pName = FindPlayerName(tile.TileObject[ns].Data1);
								int pLen = strlen(pName.c_str());

								Write(pLen,2);
								Write((char *)pName.c_str(),pLen);

								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_HPBAR),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_DIRECTION),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_OUTFIT),2);

								if(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_OUTFIT) != 0)
								{
									Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_HEAD_COLOR),1);
									Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_BODY_COLOR),1);
									Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_LEGS_COLOR),1);
									Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_FEET_COLOR),1);
									Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_ADDON),1);
								} else
								{
									Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_HEAD_COLOR),2);
								}

								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_LIGHT),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_LIGHT_COLOR),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_WALK_SPEED),2);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_SKULL),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_PARTY),1);
							} else 
							{
								Write(tile.TileObject[ns].ObjectID,2);

								if(tile.TileObject[ns].ObjectID > 99)
								{
									if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Stackable))
									{
										Write(tile.TileObject[ns].Data1,1);
									}
									if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Rune))
									{
										Write(tile.TileObject[ns].Data1,1);
									}
									if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Fluid))
									{
										Write(tile.TileObject[ns].Data1,1);
									}
									if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Splash))
									{
										Write(tile.TileObject[ns].Data1,1);
									}
								}
							}
						}

						Write(0x00,1);
						Write(0xFF,1);
					} else 
					{
						if(data[offset-2] == 0xFF)
						{
							Write(0x00,1);
							Write(0xFF,1);
						} else 
						{
							if(offset == 18)
							{
								Write(0x00,1);
								Write(0xFF,1);
							} else
							{
								data[offset-2]++;
							}
						}
					}
				}
			}
		}

	}

	Write(0x83,1);
	Write(*PLAYER_X,2);
	Write(*PLAYER_Y,2);
	Write(*PLAYER_Z,1);
	Write(0x0B,1);

	if(*PLAYER_SLOT_HEAD != 0) 
	{
		Write(0x78,1);
		Write(0x01,1);
		Write(*PLAYER_SLOT_HEAD,2);
	}

	if(*PLAYER_SLOT_NECKLACE != 0) 
	{
		Write(0x78,1);
		Write(0x02,1);
		Write(*PLAYER_SLOT_NECKLACE,2);
	}

	if(*PLAYER_SLOT_CONTAINER != 0) 
	{
		Write(0x78,1);
		Write(0x03,1);
		Write(*PLAYER_SLOT_CONTAINER,2);
	}

	if(*PLAYER_SLOT_ARMOR != 0) 
	{
		Write(0x78,1);
		Write(0x04,1);
		Write(*PLAYER_SLOT_ARMOR,2);
	}

	if(*PLAYER_SLOT_RIGHT != 0) 
	{
		Write(0x78,1);
		Write(0x05,1);
		Write(*PLAYER_SLOT_RIGHT,2);

		if(DatReader.GetObjectA(*PLAYER_SLOT_RIGHT,Stackable))
		{
			Write(*PLAYER_SLOT_RIGHT_COUNT,1);
		}
	}

	if(*PLAYER_SLOT_LEFT != 0) 
	{
		Write(0x78,1);
		Write(0x06,1);
		Write(*PLAYER_SLOT_LEFT,2);

		if(DatReader.GetObjectA(*PLAYER_SLOT_LEFT_COUNT,Stackable))
		{
			Write(*PLAYER_SLOT_LEFT_COUNT,1);
		}
	}

	if(*PLAYER_SLOT_LEGS != 0) 
	{
		Write(0x78,1);
		Write(0x07,1);
		Write(*PLAYER_SLOT_LEGS,2);
	}

	if(*PLAYER_SLOT_FEET != 0) 
	{
		Write(0x78,1);
		Write(0x08,1);
		Write(*PLAYER_SLOT_FEET,2);
	}

	if(*PLAYER_SLOT_RING != 0) 
	{
		Write(0x78,1);
		Write(0x09,1);
		Write(*PLAYER_SLOT_RING,2);
	}

	if(*PLAYER_SLOT_AMMO != 0) 
	{
		Write(0x78,1);
		Write(0x0A,1);
		Write(*PLAYER_SLOT_AMMO,2);

		if(DatReader.GetObjectA(*PLAYER_SLOT_AMMO,Stackable))
		{
			Write(*PLAYER_SLOT_AMMO_COUNT,1);
		}
	}

	Write(0x82,1);
	Write(0xFF,1);
	Write(0xD7,1);
	Write(0x8D,1);
	Write(*PLAYER_ID,4);
	Write(0x00,1);
	Write(0x00,1);

	for(int i = VIP_BEGIN; i < VIP_END; i = i + VipPlayerDist) {
		int *playerId = (int *)i;
		if(*playerId != 0) {
			string vname ((char *)(i + VipNameOffset));
			int *online = (int *)(i + VipStatusOffset);
			int namelen = strlen(vname.c_str());

			Write(0xD2,1);
			Write(*playerId,4);
			Write(namelen,2);
			Write((char *)vname.c_str(),namelen);
			Write(*online,1);
		}
	}

	Write(0xB4,1);
	Write(0x16,1);
	Write(53,2);
	Write("TCam - 1.0 - Official Site: www.tibiafreak.com       ",53);
	

	Write(0xA0,1);
	Write(*PLAYER_HP,2);
	Write(*PLAYER_HP_MAX,2);
	Write(*PLAYER_CAP,2);
	Write(*PLAYER_EXP,4);
	Write(*PLAYER_LEVEL,2);
	Write(*PLAYER_LEVEL_PER,1);
	Write(*PLAYER_MANA,2);
	Write(*PLAYER_MANA_MAX,2);
	Write(*PLAYER_MAGIC_LEVEL,1);
	Write(*PLAYER_MAGIC_LEVEL_PER,1);
	Write(*PLAYER_SOUL,1);
	Write(*PLAYER_STAMINA,2);

	Write(0xA1,1);
	Write(*PLAYER_FIST,1);
	Write(*PLAYER_FIST_PER,1);
	Write(*PLAYER_CLUB,1);
	Write(*PLAYER_CLUB_PER,1);
	Write(*PLAYER_SWORD,1);
	Write(*PLAYER_SWORD_PER,1);
	Write(*PLAYER_AXE,1);
	Write(*PLAYER_AXE_PER,1);
	Write(*PLAYER_DIST,1);
	Write(*PLAYER_DIST_PER,1);
	Write(*PLAYER_SHIELD,1);
	Write(*PLAYER_SHIELD_PER,1);
	Write(*PLAYER_FISH,1);
	Write(*PLAYER_FISH_PER,1);

	GenerateHeader(true);

	EncryptPacket(data);
	int unencLen = 0;
	memcpy(&unencLen,&data[0],2);

	FileHandler.WritePacket(data,unencLen+2,PACKET_ID);

	Flush();
	return true;
}

/*************************************************************************************************
Function name: MapPacketAbove
Purpose: Generates the mappacket for ground level and above
Comments: 
Return: True if succeeded
*************************************************************************************************/
bool CMap::MapPacketAbove()
{
	Allocate(50000);

	Write(0x0A,1);
	Write(*PLAYER_ID,4);
	Write(0x32,1);
	Write(0x00,1);
	Write(0x00,1);
	Write(0x64,1);
	Write(BattleListGet(*PLAYER_ID,BATTLELIST_X),2);
	Write(BattleListGet(*PLAYER_ID,BATTLELIST_Y),2);
	Write(BattleListGet(*PLAYER_ID,BATTLELIST_Z),1);

	for(int nz = 0; nz < 8; nz++)
	{
		for(int nx = 0; nx < 18; nx++)
		{
			for(int ny = 0; ny < 14; ny++)
			{
				int playerTileNum = GetPlayerTileNum();
				int myZ = playerTileNum / (14 * 18);
				int myY = (playerTileNum - myZ * 14 * 18) / 18;
				int myX = (playerTileNum - myZ * 14 * 18) - myY * 18;

				if(playerTileNum < 0)
					return false;

				int tempx = 17 - ((17 - (nx + myX - 8)) % 18);
				tempx = (tempx <= 18)? tempx : -((17 - (nx + myX - 8)) % 18) - 1;
				int tempy = 13 - ((13 - (ny + myY - 6)) % 14);
				tempy = (tempy <= 14)? tempy : -((13 - (ny + myY - 6)) % 14) - 1;

				if(tempx % 18 == 0)
					tempx = 0;
				if(tempy % 14 == 0)
					tempy = 0;

				STile tile;
				ZeroMemory(&tile,sizeof(tile));
				memcpy(&tile,(void *)(*MAP_POINTER + (nz * 43344) + (tempx * 172) + (tempy * 3096)),sizeof(tile));

				if(tile.Count > 0)
				{
					if(tile.Count > 13)
					{
						return false;
					}
					for(unsigned int ns = 0; ns < tile.Count; ns++)
					{

						if(tile.TileObject[ns].ObjectID == 99)
						{
							Write(0x61,1);
							Write(0x00,1);
							Write(0x00,1);
							Write(0x00,1);
							Write(0x00,1);
							Write(0x00,1);

							Write(tile.TileObject[ns].Data1,4);
							string pName = FindPlayerName(tile.TileObject[ns].Data1);
							int pLen = strlen(pName.c_str());

							Write(pLen,2);
							Write((char *)pName.c_str(),pLen);

							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_HPBAR),1);
							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_DIRECTION),1);
							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_OUTFIT),2);

							if(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_OUTFIT) != 0)
							{
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_HEAD_COLOR),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_BODY_COLOR),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_LEGS_COLOR),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_FEET_COLOR),1);
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_ADDON),1);
							} else
							{
								Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_HEAD_COLOR),2);
							}

							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_LIGHT),1);
							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_LIGHT_COLOR),1);
							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_WALK_SPEED),2);
							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_SKULL),1);
							Write(BattleListGet(tile.TileObject[ns].Data1,BATTLELIST_PARTY),1);
						} else 
						{
							Write(tile.TileObject[ns].ObjectID,2);

							if(tile.TileObject[ns].ObjectID > 99)
							{
								if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Stackable))
								{
									Write(tile.TileObject[ns].Data1,1);
								}
								if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Rune))
								{
									Write(tile.TileObject[ns].Data1,1);
								}
								if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Fluid))
								{
									Write(tile.TileObject[ns].Data1,1);
								}
								if(DatReader.GetObjectA(tile.TileObject[ns].ObjectID,Splash))
								{
									Write(tile.TileObject[ns].Data1,1);
								}
							}
						}
					}

					Write(0x00,1);
					Write(0xFF,1);
				} else 
				{
					if(data[offset-2] == 0xFF)
					{
						Write(0x00,1);
						Write(0xFF,1);
					} else 
					{
						if(offset == 18)
						{
							Write(0x00,1);
							Write(0xFF,1);
						} else
						{
							data[offset-2]++;
						}
					}
				}
			}
		}

	}

	Write(0x83,1);
	Write(*PLAYER_X,2);
	Write(*PLAYER_Y,2);
	Write(*PLAYER_Z,1);
	Write(0x0B,1);

	if(*PLAYER_SLOT_HEAD != 0) 
	{
		Write(0x78,1);
		Write(0x01,1);
		Write(*PLAYER_SLOT_HEAD,2);
	}

	if(*PLAYER_SLOT_NECKLACE != 0) 
	{
		Write(0x78,1);
		Write(0x02,1);
		Write(*PLAYER_SLOT_NECKLACE,2);
	}

	if(*PLAYER_SLOT_CONTAINER != 0) 
	{
		Write(0x78,1);
		Write(0x03,1);
		Write(*PLAYER_SLOT_CONTAINER,2);
	}

	if(*PLAYER_SLOT_ARMOR != 0) 
	{
		Write(0x78,1);
		Write(0x04,1);
		Write(*PLAYER_SLOT_ARMOR,2);
	}

	if(*PLAYER_SLOT_RIGHT != 0) 
	{
		Write(0x78,1);
		Write(0x05,1);
		Write(*PLAYER_SLOT_RIGHT,2);

		if(DatReader.GetObjectA(*PLAYER_SLOT_RIGHT,Stackable))
		{
			Write(*PLAYER_SLOT_RIGHT_COUNT,1);
		}
	}

	if(*PLAYER_SLOT_LEFT != 0) 
	{
		Write(0x78,1);
		Write(0x06,1);
		Write(*PLAYER_SLOT_LEFT,2);

		if(DatReader.GetObjectA(*PLAYER_SLOT_LEFT_COUNT,Stackable))
		{
			Write(*PLAYER_SLOT_LEFT_COUNT,1);
		}
	}

	if(*PLAYER_SLOT_LEGS != 0) 
	{
		Write(0x78,1);
		Write(0x07,1);
		Write(*PLAYER_SLOT_LEGS,2);
	}

	if(*PLAYER_SLOT_FEET != 0) 
	{
		Write(0x78,1);
		Write(0x08,1);
		Write(*PLAYER_SLOT_FEET,2);
	}

	if(*PLAYER_SLOT_RING != 0) 
	{
		Write(0x78,1);
		Write(0x09,1);
		Write(*PLAYER_SLOT_RING,2);
	}

	if(*PLAYER_SLOT_AMMO != 0) 
	{
		Write(0x78,1);
		Write(0x0A,1);
		Write(*PLAYER_SLOT_AMMO,2);

		if(DatReader.GetObjectA(*PLAYER_SLOT_AMMO,Stackable))
		{
			Write(*PLAYER_SLOT_AMMO_COUNT,1);
		}
	}

	Write(0x82,1);
	Write(0xFF,1);
	Write(0xD7,1);
	Write(0x8D,1);
	Write(*PLAYER_ID,4);
	Write(0x00,1);
	Write(0x00,1);

	
	for(int i = VIP_BEGIN; i < VIP_END; i = i + VipPlayerDist) {
		int *playerId = (int *)i;
		if(*playerId != 0) {
			string vname ((char *)(i + VipNameOffset));
			int *online = (int *)(i + VipStatusOffset);
			int namelen = strlen(vname.c_str());

			Write(0xD2,1);
			Write(*playerId,4);
			Write(namelen,2);
			Write((char *)vname.c_str(),namelen);
			Write(*online,1);
		}
	}

	Write(0xB4,1);
	Write(0x16,1);
	Write(53,2);
	Write("TCam - 1.0 - Official Site: www.tibiafreak.com       ",53);

	Write(0xA0,1);
	Write(*PLAYER_HP,2);
	Write(*PLAYER_HP_MAX,2);
	Write(*PLAYER_CAP,2);
	Write(*PLAYER_EXP,4);
	Write(*PLAYER_LEVEL,2);
	Write(*PLAYER_LEVEL_PER,1);
	Write(*PLAYER_MANA,2);
	Write(*PLAYER_MANA_MAX,2);
	Write(*PLAYER_MAGIC_LEVEL,1);
	Write(*PLAYER_MAGIC_LEVEL_PER,1);
	Write(*PLAYER_SOUL,1);
	Write(*PLAYER_STAMINA,2);

	Write(0xA1,1);
	Write(*PLAYER_FIST,1);
	Write(*PLAYER_FIST_PER,1);
	Write(*PLAYER_CLUB,1);
	Write(*PLAYER_CLUB_PER,1);
	Write(*PLAYER_SWORD,1);
	Write(*PLAYER_SWORD_PER,1);
	Write(*PLAYER_AXE,1);
	Write(*PLAYER_AXE_PER,1);
	Write(*PLAYER_DIST,1);
	Write(*PLAYER_DIST_PER,1);
	Write(*PLAYER_SHIELD,1);
	Write(*PLAYER_SHIELD_PER,1);
	Write(*PLAYER_FISH,1);
	Write(*PLAYER_FISH_PER,1);

	GenerateHeader(true);
	int unencLen = 0;
	memcpy(&unencLen,&data[0],2);

	EncryptPacket(data);

	FileHandler.WritePacket(data,unencLen+2,PACKET_ID);

	Flush();
	return true;
}

/*************************************************************************************************
Function name: Battlelist
Purpose: Writes the battlelist names to file
Comments:
Return: True always
*************************************************************************************************/
bool CMap::Battlelist()
{
	for(unsigned int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i = i + BATTLELIST_STEP) {
		int *id = (int *)(i);
		if(*id != 0)
		{
			unsigned char bList[160];
			ZeroMemory(bList,sizeof(bList));
			memcpy(&bList[0],(void *)i,160);
			FileHandler.WritePacket(bList,160,BLIST_ID);
		}
	}

	return true;
}