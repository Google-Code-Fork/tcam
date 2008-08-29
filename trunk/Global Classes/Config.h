#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>
#include <string>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib") 

class CConfig
{
public:
	/* Constructors/Destructor */
	CConfig(LPCTSTR filename);
	~CConfig();

	/* Set/Get Paths */
	BOOL SetPath(std::string tibiaVersion, char* keyName, char* path);
	BOOL GetPath(std::string tibiaVersion, char* keyName, char* buffer);


private:
	LPCTSTR fileName;
};

#endif