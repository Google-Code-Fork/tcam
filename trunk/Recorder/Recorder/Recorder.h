#ifndef RECORDER_H
#define RECORDER_H

#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include "Registry.h"
#include "..\..\Global Classes\Config.h"

using namespace std;

char RecorderDLL[MAX_PATH];

HWND MainDialog;

HWND list[100];
int nCurrentHandle;

void ModifyToken();
BOOL LoadDll();
BOOL InjectDLL(DWORD ProcessID, char *dllName);
unsigned long GetTargetProcessIdFromProcname(char *procName);
BOOL CALLBACK MessageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

#endif