// Constants
#define WIN_WIDTH			285
#define WIN_HEIGHT			230
#define IDC_LIST			1001
#define IDC_ACCEPT			1002
#define IDC_REFRESH			1003
#define PLAYER_ID			0x00626C70
#define BATTLELIST_BEGIN	0x00626CD0
#define BATTLELIST_END		0x0062CA24
#define ISONLINE			0x0077F3F8

#include "Recorder.h"
#include "resource.h"

HWND g_hWnd;
HWND hList;
DWORD dwClient[32];
HINSTANCE g_hInstance;
char szTitle[] = "Choose Client...";
char szWindowClass[] = "TCAMRECORDER";
CRegistry Registry;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG msg;
	int ret = 1;
	ModifyToken();
	CConfig config("\\config.ini");

	if(!config.GetPath("822", "Record", RecorderDLL))
	{
		GetModuleFileName(NULL, RecorderDLL, MAX_PATH);
		PathRemoveFileSpec(RecorderDLL);
		strcat(RecorderDLL, "\\TCam - Recorder.dll");
		config.SetPath("822", "Record", RecorderDLL);
	}

		if(InitializeWindow(hInstance, iCmdShow) == false)
	{
		return false;
	}


	while((ret == GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if(ret == -1)
		{
			// TODO: Handle error
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
	LoadDll();

	return 0;
}

BOOL LoadDll()
{
	DWORD ProcID = GetTargetProcessIdFromProcname("Tibia");

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

/************************************************************************************************
Full credits to Stiju from here on
*************************************************************************************************/
bool InitializeWindow(HINSTANCE hInstance, int nShowCmd)
{
	HWND hWnd;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(0, IDI_WINLOGO);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(16);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = szWindowClass;
	
	if(!RegisterClassEx(&wcex))
	{
		return false;
	}

	g_hInstance = hInstance;

	int x = (GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2;
	hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
		x, y, WIN_WIDTH, WIN_HEIGHT, 0, 0, g_hInstance, 0);

	if(!hWnd)
	{
		return false;
	}
	g_hWnd = hWnd;

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		HWND hTemp;
		HFONT hFont;
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		hList = CreateWindowEx(WS_EX_STATICEDGE, "listbox", 0, WS_CHILD|WS_VISIBLE|LBS_STANDARD|LBS_DISABLENOSCROLL, 10, 10, 260, 155, hWnd, (HMENU)IDC_LIST, g_hInstance, 0);
		SendMessage(hList, WM_SETFONT, (WPARAM) hFont, 0);
		hTemp = CreateWindowEx(0, "button", "Refresh List", WS_CHILD|WS_VISIBLE, 10, 165, 75, 23, hWnd, (HMENU)IDC_REFRESH, g_hInstance, 0);
		SendMessage(hTemp, WM_SETFONT, (WPARAM) hFont, 0);
		hTemp = CreateWindowEx(0, "button", "Ok", WS_CHILD|WS_VISIBLE, 195, 165, 75, 23, hWnd, (HMENU)IDC_ACCEPT, g_hInstance, 0);
		SendMessage(hTemp, WM_SETFONT, (WPARAM) hFont, 0);
		RefreshList();
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LIST:
			if(HIWORD(wParam) != LBN_DBLCLK)
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		case IDC_ACCEPT:
			{
				int ret = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
				if(ret != LB_ERR)
				{
					LoadDll();
					DestroyWindow(hWnd);
				}
				else
					MessageBox(hWnd, "Select a client first", szTitle, MB_ICONEXCLAMATION);
			}break;
		case IDC_REFRESH:
			RefreshList();
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void GetPlayerName(HANDLE hProcess, char *lpName)
{
	int playerID;
	ReadProcessMemory(hProcess, (LPVOID)PLAYER_ID, &playerID, 4, 0);
	for(int i = BATTLELIST_BEGIN; i < BATTLELIST_END; i += 160)
	{
		int cID;
		ReadProcessMemory(hProcess, (LPVOID)i, &cID, 4, 0);
		if(playerID == cID)
		{
			ReadProcessMemory(hProcess, (LPVOID)(i+4), lpName, 32, 0);
			return;
		}
	}
}

void RefreshList(void)
{
	HWND temp = 0;
	HANDLE hProcess;
	int x = 0;

	char buffer[128];
	char name[32];
	SendMessage(hList, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	do{
		temp = FindWindowEx(0, temp, "TibiaClient", 0);
		if(!temp)
			break;
		else
		{
			GetWindowThreadProcessId(temp, &dwClient[x]);
			hProcess = OpenProcess(PROCESS_VM_READ, false, dwClient[x]);
			int isOnline;
			ReadProcessMemory(hProcess, (LPVOID)ISONLINE, &isOnline, 4, 0);
			if(isOnline == 8)
			{
				GetPlayerName(hProcess, name);
				sprintf_s(buffer, 128, "[0x%.4X] %s", dwClient[x], name);
			}
			else
			{
				sprintf_s(buffer, 128, "[0x%.4X] Unknown", dwClient[x]);
			}
			SendMessage(hList, LB_INSERTSTRING, (WPARAM)(-1), (LPARAM)buffer);
			CloseHandle(hProcess);
			x++;
		}
	}while(true);
	SendMessage(hList, LB_SETCURSEL, 0, 0);
}