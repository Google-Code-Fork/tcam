#ifndef TCAM_H
#define TCAM_H

#include "Globals.h"
#include "FileHandler.h"
#include "Tibia.h"
#include "Packet.h"
#include "XTEA.h"

bool camRecording = false;
bool fileNameSet = false;
bool genMapPacket = false;
char fileName[MAX_PATH];

HWND MainDialog;
HWND TibiahWnd;
HINSTANCE hInstance;
HANDLE hMainThread;

int TotalRecTime;
clock_t start;
int delay;

#endif