#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <list>
#include <stdio.h>
#include <commctrl.h>
#include <process.h>

using namespace std;

struct SPosition
{
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct SPacketBuffer
{
	char *buf;
	unsigned int nSize;
};

struct STileObject { 
	unsigned int ObjectID; 
	unsigned int Data1; 
	unsigned int Data2; 
};

struct STile { 
	unsigned int Count; 
	STileObject TileObject[13];
	unsigned int Unknown1; 
	unsigned int Unknown2; 
	unsigned int Unknown3;
};


extern HWND MainDialog;

extern class CFileHandler FileHandler;

#endif