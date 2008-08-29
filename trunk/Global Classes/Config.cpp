#include "Config.h"

CConfig::CConfig(LPCTSTR filename)
{
	/* Config file is stored to the same directory as the .exe */
	char *fbuffer = new char[MAX_PATH];
	GetModuleFileName(NULL, fbuffer, MAX_PATH);
	PathRemoveFileSpec(fbuffer);

	strcat(fbuffer, filename);
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

	
