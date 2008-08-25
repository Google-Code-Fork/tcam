// Credits to Unknown?

#include "Registry.h"

CRegistry::CRegistry()
{
	
}

CRegistry::~CRegistry()
{
	
}

BOOL CRegistry::QueryStringValue(const HKEY root,LPCSTR path,LPCSTR key,LPSTR buffer,DWORD &Length)
{
	HKEY hKey;
	DWORD type=0;
	BOOL returnval=TRUE;
	if (RegOpenKeyExA(root,path,0,KEY_QUERY_VALUE,&hKey) != ERROR_SUCCESS)
	{
		*buffer=0;
		Length=0;
		return FALSE;
	}
	if (RegQueryValueExA(hKey,key,NULL,&type,(LPBYTE)buffer,&Length) != ERROR_SUCCESS)
	{
		returnval=FALSE;
	}
	RegCloseKey(hKey);
	return returnval;
}

BOOL CRegistry::CreateKey(const HKEY root,LPCSTR path,LPCSTR key,LPCSTR buffer,const DWORD length)
{
	HKEY hKey;
	DWORD type=REG_SZ;
	BOOL returnval=TRUE;
	if (RegCreateKeyExA(root,path,0,NULL,0 ,KEY_ALL_ACCESS,NULL,&hKey,&type) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	type=REG_SZ;
	if (RegSetValueExA(hKey,key,NULL,type,(LPBYTE)buffer,length) != ERROR_SUCCESS)
	{
		returnval = FALSE;
	}
	RegCloseKey(hKey);
	return returnval;
}