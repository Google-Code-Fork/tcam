#ifndef TIBIA_H
#define TIBIA_H

#include "Globals.h"
#include "XTEA.h"
#include "Constants.h"

class CTibia: public XTEA
{
public:
	CTibia();
	~CTibia();

	char *CreateCharList();
	void ChangeFPS(double dFPS);
	void UpdateFPS();

	double dOldFPS;

private:
};
#endif