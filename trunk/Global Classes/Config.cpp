#include "Config.h"

CConfig::CConfig(LPCTSTR filename)
{
	/* Config file is stored to the same directory as the .exe */
	LPCTSTR fbuffer = new char[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, (LPSTR)fbuffer);
	strcat((char*)fbuffer, filename);

	fileName = fbuffer;
	
}

CConfig::~CConfig()
{
	delete [] fileName;
}

BOOL CConfig::SetPath(std::string tibiaVersion, char* keyName, char* path)
{
	tibiaVersion.insert(0, "Tibia");
	return WritePrivateProfileString(tibiaVersion.c_str(), keyName, path, fileName);
}

BOOL CConfig::GetPath(std::string tibiaVersion, char* keyName, char* buffer)
{
	tibiaVersion.insert(0, "Tibia");
	return GetPrivateProfileString(tibiaVersion.c_str(), keyName, NULL, buffer, MAX_PATH, fileName);
}

	
