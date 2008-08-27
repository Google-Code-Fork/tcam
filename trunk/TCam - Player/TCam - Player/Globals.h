#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

#pragma comment(lib, "wsock32.lib") // Include winsock

enum Protocol
{
	XTEA_ID = 0x30,
	BLIST_ID = 0x31,
	DELAY_ID = 0x32,
	PACKET_ID = 0x40
};

struct SPacket
{
	Protocol nID;
	unsigned char cBuffer[20000];
	int nSize;
};

struct SRectScreen
{
	int Unknown1;
	int Unknown2;
	int Unknown3;
	int Unknown4;
	int Unknown5;
	int posY;
	int posX;
	int sWidth;
	int sHeight;
};

struct SPosition
{
	int pX;
	int pY;
	int pZ;
};

extern HWND MainDialog;
extern HWND hWnd;
extern class CNetworkClient NetworkClient; 

#endif