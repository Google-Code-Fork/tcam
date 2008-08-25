#ifndef TIBIA_H
#define TIBIA_H

#include "Globals.h"

class CTibia
{
public:
	CTibia(void);
	~CTibia(void);
	
	int BattleListGet(int cid,int offset);	
	int GetPlayerTileNum();
	string FindPlayerName(int cid);

private:
};

#endif