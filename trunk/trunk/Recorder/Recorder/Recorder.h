#ifndef RECORDER_H
#define RECORDER_H

#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include "Registry.h"

using namespace std;

char RecorderDLL[MAX_PATH];

void ModifyToken();
BOOL LoadDll();
BOOL InjectDLL(DWORD ProcessID, char *dllName);
unsigned long GetTargetProcessIdFromProcname(char *procName);

#endif