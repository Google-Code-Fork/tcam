#ifndef REGISTRY_H
#define REGISTRY_H

#include "Globals.h"

class CRegistry  
{
public:
	CRegistry();
	~CRegistry();
	BOOL QueryStringValue(const HKEY root,LPCSTR path,LPCSTR key,LPSTR buffer,DWORD &Length);
	BOOL CreateKey(const HKEY root,LPCSTR path,LPCSTR key,LPCSTR buffer,const DWORD length);
private:
};

#endif