#define WM_TRAY (WM_USER + 1)

#include "TCam.h"

CFileHandler FileHandler;
CPacket Packet;
CTibia Tibia;
CMap Map;

int __stdcall SendHook(SOCKET s, const char *buf, int len, int flags) //Our Send function
{
	while(genMapPacket)
		Sleep(1);

	int SentBytes = SendPacket(s,buf,len,flags); // The return statement to original ws2_32.dll Send function
	TibiaSoc = s; // Store Tibias socket

	return SentBytes; //Return the num of bytes sent
}

int __stdcall RecvHook(SOCKET s, const char *buf, int len, int flags) //Our Recv function
{
	while(genMapPacket)
		Sleep(1);

	int RecvedBytes = RecvPacket(s,buf,len,flags);
	TibiaSoc = s;

	if(bRecording)
	{
		Packet.PhrasePacket((unsigned char *)buf,RecvedBytes);
	}

	return RecvedBytes;

}

bool OpenFileDialog( char *file )	//Function that will call browse dialog
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
	OpenFileName.lpstrFilter = "TCam Recording (*.TCam)\0*.tcam\0\0";
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter = 0;
	OpenFileName.nFilterIndex = 0;
	OpenFileName.lpstrFile = szFile;
	OpenFileName.nMaxFile = sizeof( szFile );
	OpenFileName.lpstrFileTitle = NULL;
	OpenFileName.nMaxFileTitle = 0;
	OpenFileName.lpstrInitialDir = CurrentDir;
	OpenFileName.lpstrTitle = "Recording filename...";
	OpenFileName.nFileOffset = 0;
	OpenFileName.nFileExtension = 0;
	OpenFileName.lpstrDefExt = "tcam";
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

BOOL CALLBACK MessageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	MainDialog = hwnd;

	switch(Message)
	{

	case WM_INITDIALOG:
		{
			Tibia.SetText("TCam 0.5");
			trayIcon.cbSize = sizeof( NOTIFYICONDATA );
			trayIcon.uCallbackMessage = WM_TRAY;
			trayIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
			trayIcon.uID = 1;
			trayIcon.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_FIRE);
			strcpy( trayIcon.szTip, "Click to restore TCam Recorder");
			trayIcon.hWnd = MainDialog;
			Shell_NotifyIcon( NIM_ADD, &trayIcon ); 
		}
		break;

	case WM_TRAY:
		if ((UINT)lParam == WM_LBUTTONDOWN || (UINT)lParam == WM_LBUTTONDBLCLK || (UINT)lParam == WM_RBUTTONDOWN || (UINT)lParam == WM_RBUTTONDBLCLK)
		{
			if (IsWindowVisible(MainDialog))
			{
				ShowWindow(MainDialog,SW_HIDE); 
			}
			else
			{
				ShowWindow(MainDialog,SW_SHOW); 
			}
		}
		break;
	case WM_CLOSE:
		{
			ShowWindow(MainDialog,SW_HIDE);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch(wmId)
		{

		case IDC_RECORD:
			{
				if(bRecording)
				{
					SetWindowText(GetDlgItem(MainDialog,IDC_RECORD),"Record");
					fileNameSet = false;
					genMapPacket = false;
					bRecording = false;

					FileHandler.Write(TotalRecTime,4);
					FileHandler.Export();
				} else 
				{
					if(!fileNameSet)
					{
						if(OpenFileDialog(fileName))
						{
							Tibia.SetText("Awaiting Login...");

							while(*PLAYER_CONNECTION != 8) { Sleep(1); }

							genMapPacket = true;
							FileHandler.Open(fileName);
							FileHandler.WritePacket((unsigned char *)XTeaAddress,16,XTEA_ID);

							if(Tibia.BattleListGet(*PLAYER_ID,BATTLELIST_Z) <= 7)
							{
								if(!Map.MapPacketAbove())
								{
									break;
								}
							} else
							{
								if(!Map.MapPacketBelow())
								{
									break;
								}
							}

							Map.Battlelist();

							bRecording = true;
							genMapPacket = false;

							SetWindowText(GetDlgItem(MainDialog,IDC_RECORD),"Stop");
							Tibia.SetText("Recording");
						}
					}
				}
			}
			break;

		case IDC_VISITSITE:
			{
				ShellExecute( NULL, "open", "explorer", "http://www.tibiafreak.com", NULL, SW_SHOWMAXIMIZED );
			}
			break;
		}

		break;
	case WM_DESTROY:
		break;

	}
	return 0;
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MessageHandler);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) //Decide what to do
	{
	case DLL_PROCESS_ATTACH: //On dll attach
		{
			TotalRecTime = 0;

			hInstance = (HINSTANCE)hModule;
			hMainThread = CreateThread(NULL, 0, MainThread, 0, 0, 0); // Create MainDialog GUI

			SendPacket = (FUNC_SENDBEGIN)DetourFunction((PBYTE)DetourFindFunction("ws2_32.dll", "send"), (PBYTE)SendHook); //Set hook on send()
			RecvPacket = (FUNC_RECVBEGIN)DetourFunction((PBYTE)DetourFindFunction("ws2_32.dll", "recv"), (PBYTE)RecvHook); //Set hook on recv()

			break;
		}
	case DLL_THREAD_ATTACH: //On thread attach
		break;
	case DLL_THREAD_DETACH: //On thread detach
		break;
	case DLL_PROCESS_DETACH: //on process detach
		break;
	}
	return TRUE;
}
