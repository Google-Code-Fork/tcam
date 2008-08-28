#ifndef TIBIA_H
#define TIBIA_H

#include "Globals.h"
#include "XTEA.h"

class CTibia: public XTEA
{
public:
	CTibia();
	~CTibia();

	char *CreateCharList();

private:
};
#endif