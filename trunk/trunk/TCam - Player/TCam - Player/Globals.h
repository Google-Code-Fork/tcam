#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

#pragma comment(lib, "wsock32.lib") // Include winsock

struct CamPacket
{
	char id;
	char buf[20000];
	int nSize;
	long delay;
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

#endif