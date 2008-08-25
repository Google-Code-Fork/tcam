#include "Recorder.h"

CRegistry Registry;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	ModifyToken();
	DWORD Len = 260;
	if(!Registry.QueryStringValue(HKEY_LOCAL_MACHINE,"Software\\TibiaFreak\\TCam", "Record",RecorderDLL,Len))
	{
		MessageBox(NULL, "Could not find path to DLL \n Please reinstall the application", "Loader", NULL);
		exit(1);
	}
	LoadDll();

	return 0;
}

BOOL LoadDll()
{
	DWORD ProcID = GetTargetProcessIdFromProcname("Tibia");;



	if(!(InjectDLL(ProcID, RecorderDLL)))
		MessageBox(NULL, "Injection Failed", "Loader", NULL);
	
	return true;
}

BOOL InjectDLL(DWORD ProcessID, char *dllName)
{
	HANDLE Proc;
	char buf[50]={0};
	LPVOID RemoteString, LoadLibAddy;

	if(!ProcessID)
		return false;

	Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

	if(!Proc)
	{
		sprintf(buf, "OpenProcess() failed: %d", GetLastError());
		MessageBox(NULL, buf, "Loader", NULL);
		return false;
	}

	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(dllName), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName, strlen(dllName), NULL);
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);	
	
	CloseHandle(Proc);

	return true;
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

unsigned long GetTargetProcessIdFromProcname(char *procName)
{
   PROCESSENTRY32 pe;
   HANDLE thSnapshot;
   BOOL retval, ProcFound = false;

   thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   if(thSnapshot == INVALID_HANDLE_VALUE)
   {
      MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", NULL);
      return false;
   }

   pe.dwSize = sizeof(PROCESSENTRY32);

    retval = Process32First(thSnapshot, &pe);

   while(retval)
   {
      if(strstr(pe.szExeFile, procName) )
      {
         ProcFound = true;
         break;
      }

      retval    = Process32Next(thSnapshot,&pe);
      pe.dwSize = sizeof(PROCESSENTRY32);
   }

   return pe.th32ProcessID;
}