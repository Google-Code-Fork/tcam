#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include "..\..\Global Classes\Config.h"

#pragma comment(lib, "version.lib") // Used for checking Tibias version

#include "detours.h"
#pragma comment(lib, "detours.lib") // Include detours library

#include "Registry.h"
using namespace std;

char camFilePath[MAX_PATH];
char TibiaPath[MAX_PATH];
char TibiaDir[MAX_PATH];
char PlayerDLL[MAX_PATH];

void ModifyToken();
bool OpenFileDialog(char *file);
bool CheckVersion(char *fName);

#endif