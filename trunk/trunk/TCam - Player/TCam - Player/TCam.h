#ifndef TCAM_H
#define TCAM_H

#include "Globals.h"
#include "NetworkClient.h"
#include "TCamReader.h"
#include "XTEA.h"
#include "Registry.h"

char fileName[MAX_PATH];
char camFullPath[MAX_PATH];

HINSTANCE hInstance;
HANDLE hMainThread;
HANDLE hWindowUpdate;
HANDLE hUpdateTimers;
HANDLE hUserInteraction;

HWND MainDialog;
HWND hWnd;
WNDPROC wndProc;

clock_t initClock = 0;
clock_t waitTime = 0;
clock_t lastWait = 0;

int cHour = 0,cMin = 0,cSec = 0,tHour = 0,tMin = 0,tSec = 0;
double speed = 1.00;

bool Playing = false;
bool FullScreen = false;

RECT workArea;
SPosition ScreenPosition;
SRectScreen InitScreenSize;

int CreateCharList(unsigned char *buffer, char *name);
LRESULT APIENTRY TibiaHwNd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL QueryStringValue(const HKEY root,LPCSTR path,LPCSTR key,LPSTR buffer,DWORD &Length);
BOOL CreateKey(const HKEY root,LPCSTR path,LPCSTR key,LPCSTR buffer,const DWORD length);

DWORD WINAPI WindowUpdate(LPVOID lpParam);
DWORD WINAPI UpdateTimers(LPVOID lpParam);
DWORD WINAPI UserInteraction(LPVOID lpParam);

#endif