#include "Tibia.h"
#include "Constants.h"

CTibia::CTibia(void)
{
	TibiahWnd = FindWindowA("TibiaClient", "Tibia");
}

CTibia::~CTibia(void)
{
}

/*************************************************************************************************
Function name: BattleListGet
Date of creation: August 18
Purpose: Return the offset value from the battlelist
Comments: 
*************************************************************************************************/
int CTibia::BattleListGet(int cid,int offset)
{
	for(unsigned int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i = i + BATTLELIST_STEP) {
		int *temp = (int *)(i);
		if(*temp == cid) {
			int *ret = (int *)(i + offset);
			return *ret;
		}
	}
	return 0;
}

/*************************************************************************************************
Function name: FindPlayerName
Date of creation: August 18
Purpose: Find a players ID
Comments: Returns 0 if player not in battlelist
*************************************************************************************************/
string CTibia::FindPlayerName(int cid) {

	for(unsigned int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i = i + BATTLELIST_STEP) {
		int *creatureId = (int *)i;
		if(*creatureId == cid) {
			char temp[32];
			memcpy(&temp[0],(void *)(i + BATTLELIST_NAME),32);
			string bname(temp);
			return bname;

		}
	}
	string error = "";
	return error;
}

/*************************************************************************************************
Function name: GetPlayerTileNum
Date of creation: August 16
Purpose: Get tile num of player
Comments: 
*************************************************************************************************/
int CTibia::GetPlayerTileNum()
{
	for(int i = 0; i < 2016; i++) // Loop through all tiles
	{ 
		int h = (MapTileDist * i);
		int *stackSize = (int *)(*MAP_POINTER + h); // Get the number of objects in tile
		if(*stackSize > 1) // If more then 1 then there must be at least 2 objects (Player + tile)
		{ 
			for(int e = 0; e < (*stackSize); e++) // Loop through all objects
			{ 
				int *objectID = (int *)(*MAP_POINTER + (i * MapTileDist) + (e * MapObjectDist) + MapObjectIdOffset);
				if(*objectID == 99) 
				{ 
					int *data = (int *)(*MAP_POINTER + (i * MapTileDist) + (e * MapObjectDist) + MapObjectDataOffset);
					if(*data == *PLAYER_ID) 
					{
						return i;
					}
				}
			}
		}
	}
	return -1;
}

/*************************************************************************************************
Function name: BattleListGet
Date of creation: August 18
Purpose: Return the offset value from the battlelist
Comments: 
*************************************************************************************************/
void CTibia::SetText(char *cBuffer)
{
	SetWindowText(TibiahWnd, cBuffer);
}