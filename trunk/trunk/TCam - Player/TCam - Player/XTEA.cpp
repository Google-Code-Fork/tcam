#include "XTEA.h"

XTEA::XTEA()
{
}

XTEA::~XTEA()
{
}

bool XTEA::EncryptPacket(unsigned char *cBuffer)
{
	unsigned int nSize = 0;
	memcpy(&nSize,&cBuffer[0],2);

	if(nSize % 8 != 0)
		return false;

	for(unsigned int i = 2; i < (nSize + 2); i += 8) 
	{
		encipher(32, (unsigned long*)&cBuffer[i], XTeaAddress);
	}

	return true;
}


bool XTEA::DecryptPacket(unsigned char *cBuffer)
{
	unsigned int nSize = 0;
	memcpy(&nSize,&cBuffer[0],2);

	if(nSize % 8 != 0)
		return false;

	for(unsigned int i = 2; i < (nSize + 2); i += 8) 
	{
		decipher(32, (unsigned long*)&cBuffer[i], XTeaAddress);
	}

	return true;
}

/*
The following code was taken from Wikipedia.
Full credits to David Wheeler and Roger Needham.
*/
void XTEA::encipher(unsigned int num_rounds, unsigned long* v, unsigned long* k) 
{
	unsigned long v0=v[0], v1=v[1], i;
	unsigned long sum=0, delta=0x9E3779B9;
	for(i=0; i<num_rounds; i++) {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
		sum += delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
	}
	v[0]=v0; v[1]=v1;
}

void XTEA::decipher(unsigned int num_rounds, unsigned long* v, unsigned long* k) 
{
	unsigned long v0=v[0], v1=v[1], i;
	unsigned long delta=0x9E3779B9, sum=delta*num_rounds;
	for(i=0; i<num_rounds; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
		sum -= delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
	}
	v[0]=v0; v[1]=v1;
}