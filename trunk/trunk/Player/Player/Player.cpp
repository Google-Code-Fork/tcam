#include "Player.h"

CRegistry Registry;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	CConfig config("\\config.ini");
	if(strlen(szCmdLine) != NULL)
	{
		ModifyToken();

		memcpy(&camFilePath[0],&szCmdLine[1],strlen(szCmdLine)-2); // Get path to cam file

		Registry.CreateKey(HKEY_LOCAL_MACHINE, "Software\\TibiaFreak\\TCam", "CamPlaying", camFilePath,strlen(camFilePath));

		DWORD Len = 260;
		if(!config.GetPath("822", "TibiaPath", TibiaPath))
		{
			while(1)
			{
				if(OpenFileDialog(TibiaPath))
				{
					if(CheckVersion(TibiaPath))
					{
						config.SetPath("822", "TibiaPath", TibiaPath);
						break;
					}
				}
			}
		} else
		{
			if(!CheckVersion(TibiaPath))
			{
				MessageBoxA(0,"Wrong Tibia version!","Error",MB_OK);
				exit(1);
			}
		}

		if(!config.GetPath("822", "Player", PlayerDLL))
		{
			//Get the .exe dir
			GetModuleFileName(NULL, PlayerDLL, MAX_PATH);
			PathRemoveFileSpec(PlayerDLL);
			strcat(PlayerDLL, "\\TCam - Player.dll");
			config.SetPath("822", "Player", PlayerDLL);
		}

		memcpy(&TibiaDir[0],&TibiaPath[0],MAX_PATH);
		strrchr(TibiaDir, '\\')[0] = '\0';


		if( GetFileAttributes(PlayerDLL) == 0xFFFFFFFF )
		{
			MessageBoxA(0,"Could not find \"TCam - Player.dll\" \n Please re-install the application.","Error",MB_OK);
			return 0;
		}


		STARTUPINFO si = {sizeof(STARTUPINFO)};
		PROCESS_INFORMATION pi = {0};

		std::stringstream CmdArgsStream;
		CmdArgsStream << " gamemaster " << "-camfile:" << camFilePath;

		BOOL bSuccess = DetourCreateProcessWithDll( TibiaPath, (LPTSTR)CmdArgsStream.str().c_str(), 0, 0, TRUE,
			CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
			TibiaDir, &si, &pi, PlayerDLL, 0 );

		
		if (!bSuccess)
		{
			MessageBoxA(0,"Could not create process.","Error",MB_OK);
		} else
		{
			SetForegroundWindow((HWND)pi.hProcess);
		}

	} else
	{
		MessageBoxA(0,"To view a TCam, you must double click on the .tcam file!","Error",MB_OK);
	}

	return 0;
}

void ModifyToken()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
	}
}

bool OpenFileDialog(char *file)	//Function that will call browse dialog
{
	OPENFILENAME OpenFileName;
	char szFile[MAX_PATH];
	ZeroMemory(szFile,sizeof(szFile));
	char CurrentDir[MAX_PATH];
	ZeroMemory(CurrentDir,sizeof(CurrentDir));

	szFile[0] = 0;
	GetCurrentDirectory( MAX_PATH, CurrentDir );

	OpenFileName.lStructSize = sizeof( OPENFILENAME );
	OpenFileName.hwndOwner = NULL;
	OpenFileName.lpstrFilter = "Tibia 8.22 (*.exe)\0*.exe\0\0";
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter = 0;
	OpenFileName.nFilterIndex = 0;
	OpenFileName.lpstrFile = szFile;
	OpenFileName.nMaxFile = sizeof( szFile );
	OpenFileName.lpstrFileTitle = NULL;
	OpenFileName.nMaxFileTitle = 0;
	OpenFileName.lpstrInitialDir = CurrentDir;
	OpenFileName.lpstrTitle = "Select path to Tibia 8.22";
	OpenFileName.nFileOffset = 0;
	OpenFileName.nFileExtension = 0;
	OpenFileName.lpstrDefExt = "exe";
	OpenFileName.lCustData = 0;
	OpenFileName.lpfnHook = NULL;
	OpenFileName.lpTemplateName = NULL;
	OpenFileName.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;

	if( GetOpenFileName( &OpenFileName ) )
	{
		strcpy( file, szFile );
		return true;
	}
	else
		return false;

}

bool CheckVersion(char *fName)
{
	DWORD dwHandle, dwLen;
	UINT BufLen;
	LPTSTR lpData;
	VS_FIXEDFILEINFO *pFileInfo;
	dwLen = GetFileVersionInfoSize( fName, &dwHandle );
	if (!dwLen) 
		return false;

	lpData = (LPTSTR) malloc (dwLen);
	if (!lpData) 
		return false;

	if( !GetFileVersionInfo( fName, dwHandle, dwLen, lpData ) )
	{
		free (lpData);
		return false;
	}
	if( VerQueryValue( lpData, "\\", (LPVOID *) &pFileInfo, (PUINT)&BufLen ) ) 
	{
		if((int)HIWORD(pFileInfo->dwFileVersionMS) != 8)
		{
			MessageBoxA(0,"Error: Invalid Tibia file version\nPlease select path to Tibia 8.22","Info",MB_OK);
			free (lpData);
			return false;
		}
		if((int)LOWORD(pFileInfo->dwFileVersionMS) != 2)
		{
			MessageBoxA(0,"Error: Invalid Tibia file version\nPlease select path to Tibia 8.22","Info",MB_OK);
			free (lpData);
			return false;
		}
		if((int)HIWORD(pFileInfo->dwFileVersionLS) != 2)
		{
			MessageBoxA(0,"Error: Invalid Tibia file version\nPlease select path to Tibia 8.22","Info",MB_OK);
			free (lpData);
			return false;
		}

		free (lpData);
		return true;
	}
	free (lpData);
	return false;
}