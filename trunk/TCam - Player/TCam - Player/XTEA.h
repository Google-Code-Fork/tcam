#ifndef XTEA_H
#define XTEA_H

#include "Globals.h"
#include "Constants.h"

class XTEA 
{
public:
	XTEA();
	~XTEA();

	bool EncryptPacket(unsigned char *cBuffer);
	bool DecryptPacket(unsigned char *cBuffer);

	void encipher(unsigned int num_rounds, unsigned long *v, unsigned long *k);
	void decipher(unsigned int num_rounds, unsigned long *v, unsigned long *k);

private:
};

#endif