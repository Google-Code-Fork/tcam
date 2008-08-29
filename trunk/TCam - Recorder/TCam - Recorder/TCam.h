#ifndef TCAM_H
#define TCAM_H

#include "resource.h"

#include "Globals.h"
#include "FileHandler.h"
#include "Tibia.h"
#include "Map.h"
#include "Packet.h"

#include "detours.h" 
#pragma comment(lib, "detours.lib") // Include detours library

bool bRecording = false;
bool fileNameSet = false;
bool genMapPacket = false;
char fileName[MAX_PATH];

HWND MainDialog;
HINSTANCE hInstance;
HANDLE hMainThread;

int TotalRecTime;
clock_t start;
int delay;

NOTIFYICONDATA trayIcon; // Tray icon
BOOL CALLBACK CreditsHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

typedef int (APIENTRY __stdcall* FUNC_SENDBEGIN)(SOCKET s,const char *buf,int len,int flags); //Send function prototype
typedef int (APIENTRY __stdcall* FUNC_RECVBEGIN)(SOCKET s,const char *buf,int len,int flags); //Recv function prototype
FUNC_RECVBEGIN RecvPacket; // Define pointer to real recv() function inside Tibia
FUNC_SENDBEGIN SendPacket; // Define pointer to real send() function inside Tibia
SOCKET TibiaSoc; // Tibia Socket used

#endif