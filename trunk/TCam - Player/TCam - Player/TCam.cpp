#include "TCam.h"
#include "resource.h"

CNetworkClient NetworkClientCharList; // Used to establish connection with Login Server
CNetworkClient NetworkClient; // Used as Game Server emulator
CRegistry Registry;
CTCamReader Cam;
CTibia Tibia;

DWORD WINAPI MainThread(LPVOID lpParam)
{
	HWND tempH = 0;
	while(!tempH)
	{
		tempH = FindWindowEx(0,tempH,"TibiaClient",NULL);
		DWORD pID = 0;
		GetWindowThreadProcessId(tempH,&pID);
		if(pID == GetCurrentProcessId())
		{
			hWnd = tempH;
			break;
		} else
		{
			tempH = 0;
		}
	}

	wndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)TibiaHwNd); // Set message pointer

	while(!IsWindowVisible(hWnd)) // Loop until the window is visible
		Sleep(1);

	Sleep(1000);

	NetworkClientCharList.Setup(7151,true);
	NetworkClient.Setup(7152,false);

	if(!NetworkClientCharList.proxyReady) // Check if everything is set up
	{
		MessageBoxA(MainDialog,"Failed to initialize proxy.","Error",MB_OK);
		return 1;
	}

	RECT WindowSize;
	GetClientRect(hWnd,&WindowSize); // Get size of the window

	int posX = 120; // Distance to enter game button from left of window
	int posY = WindowSize.bottom - 220; // Distance from the bottom

	SendMessageA(hWnd,WM_LBUTTONDOWN,MAKEWPARAM(posX,posY),MAKELPARAM(posX,posY)); // Click  the "Enter Game" button
	SendMessageA(hWnd,WM_LBUTTONUP,MAKEWPARAM(posX,posY),MAKELPARAM(posX,posY));

	while(*PopupWindow != 11)
		Sleep(1);	

	SendMessage(hWnd,WM_KEYDOWN,VK_RETURN,0); // Hit Enter button
	SendMessage(hWnd,WM_CHAR,VK_RETURN,0);
	SendMessage(hWnd,WM_KEYUP,VK_RETURN,0);

	while(1) // Check start listening for connections
	{
		if(NetworkClientCharList.StartListenClient())
			break; // connection established
	}

	Sleep(100);

	NetworkClientCharList.SendMessageClient(Tibia.CreateCharList(),34); // Send login packet to client

	if(!NetworkClient.proxyReady) // Check if everyhtings OK
	{
		MessageBoxA(MainDialog,"Failed to initialize proxy.","Error",MB_OK);
		return 1;
	}

	while(*PopupWindow != 11)
		Sleep(1);	

	while(*PLAYER_CONNECTION < 4)
		Sleep(1);	

	Sleep(100);

	SendMessage(hWnd,WM_KEYDOWN,VK_RETURN,0); // Hit Enter to select Character
	SendMessage(hWnd,WM_CHAR,VK_RETURN,0);
	SendMessage(hWnd,WM_KEYUP,VK_RETURN,0);

	while(1)
	{
		if(NetworkClient.StartListenClient()) // Lisen for game connection
			break; // Established connection to Proxy
	}

	if(strcmp(camFullPath, "") == 0)
	{
		DWORD Len = 260; 
		Registry.QueryStringValue(HKEY_LOCAL_MACHINE,"Software\\TibiaFreak\\TCam", "CamPlaying",camFullPath,Len); // Get cam full-path
	}

	// Start reading file and playing cam
	Cam.Open(camFullPath);

	int myCamTime = (Cam.nTotalPlayTime / 1000); // Calcualte total running time
	tSec = myCamTime % 60;
	tMin = (myCamTime / 60) % 60;
	tHour = (myCamTime / 60) / 60;

	while(1)
	{
		Cam.SendNextPacket();
	}

	return 0;
}

LRESULT APIENTRY TibiaHwNd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // Hook to tibias message handler
{

	switch(uMsg)
	{

	case WM_KEYDOWN:
		{
			if(wParam == VK_UP) // up arrow pressed
			{
				Cam.nSpeed = 50.0;
				
				wParam = NULL;
			}
			else if(wParam == VK_LEFT) // left
			{
				if(Cam.nSpeed <= 1.0)
				{
					if(Cam.nSpeed != 0.1)
						Cam.nSpeed -= 0.1;
				}
				else
				{ 
					Cam.nSpeed -= 1.0;
				}
				wParam = NULL;
			}
			else if(wParam == VK_DOWN) // down
			{
				Cam.nSpeed = 1.00;
				wParam = NULL;
			}
			else if(wParam == VK_RIGHT) // right
			{
				if(Cam.nSpeed >= 50.0)
				{
					Cam.nSpeed = 50.0;
				} 
				else if(Cam.nSpeed >= 1.0)
				{
					Cam.nSpeed += 1.0;
				}
				else
				{
					Cam.nSpeed += 0.1;
				}
				wParam = NULL;
			} 
			else if(wParam == VK_BACK)
			{
				Cam.nSpeed = 50.0;
				Cam.Reset(Cam.nCurrentPlayTime - (30 * 1000));
			}
			else if(wParam == VK_DELETE)
			{

				if (SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0))
				{
					ScreenPosition.pX = GetSystemMetrics(SM_CXSCREEN); // Screen Height
					ScreenPosition.pY = (workArea.bottom - workArea.top); // Screen Width
				}  

				static int *ScreenPtr2 = (int *)(*ScreenPtr1 + 0x44);
				static int *ScreenPtr3 = (int *)(*ScreenPtr2 + 0x10);
				static int *ScreenPtr4 = (int *)(*ScreenPtr3 + 0x24);

				SRectScreen *ScreenStruct = (SRectScreen *)(*ScreenPtr4);

				int *ChatPtr1 = (int *)(*ScreenPtr1 + 0x40);
				int *ChatPtr2 = (int *)(*ChatPtr1 + 0x10);
				int *ChatBar = (int *)(*ChatPtr2 + 0x18);

				if(FullScreen)
				{
					ScreenStruct->posX = InitScreenSize.posX;
					ScreenStruct->posY = InitScreenSize.posY;
					ScreenStruct->sHeight = InitScreenSize.sHeight;
					ScreenStruct->sWidth = InitScreenSize.sWidth;
					*ChatBar = ScreenPosition.pZ;

					FullScreen = false;
				} else
				{
					InitScreenSize.posX = ScreenStruct->posX;
					InitScreenSize.posY = ScreenStruct->posY;
					InitScreenSize.sHeight = ScreenStruct->sHeight;
					InitScreenSize.sWidth = ScreenStruct->sWidth;

					ScreenPosition.pZ = *ChatBar;

					ScreenStruct->sHeight = ScreenPosition.pY;
					ScreenStruct->sWidth = ScreenPosition.pX;
					ScreenStruct->posX = 0;
					ScreenStruct->posY = 0;

					*ChatBar = 9999;

					FullScreen = true;
				}
			}
		}
	}

	if(Cam.nSpeed < 0.1)
		Cam.nSpeed = 0.1;

	return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam ); // return the message
} 

DWORD WINAPI UserInteraction(LPVOID lpParam)
{
	while(1)
	{
		if(!Cam.bReset)
		{
			int SkipTo = NetworkClient.RecMessageClient();

			if(SkipTo > 0 && SkipTo < Cam.nTotalPlayTime)
			{
				Cam.nSpeed = 50.0;
				Cam.Reset(SkipTo);
			} 
		}
		Sleep(10);
	}
}

DWORD WINAPI WindowUpdate(LPVOID lpParam)
{
	while(1)
	{
			cSec = (Cam.nCurrentPlayTime / 1000) % 60;
			cMin = ((Cam.nCurrentPlayTime / 1000) / 60) % 60;
			cHour = ((Cam.nCurrentPlayTime / 1000) / 60) / 60;

			char TaskBarText[100];
			sprintf(&TaskBarText[0],"TCam - %.1fx - ",Cam.nSpeed); // Format time

			if(cHour < 10)
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"0%d:",cHour);
			} else
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"%d:",cHour);
			}

			if(cMin < 10)
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"0%d:",cMin);
			} else
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"%d:",cMin);
			}

			if(cSec < 10)
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"0%d / ",cSec);
			} else
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"%d / ",cSec);
			}

			if(tHour < 10)
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"0%d:",tHour);
			} else
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"%d:",tHour);
			}

			if(tMin < 10)
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"0%d:",tMin);
			} else
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"%d:",tMin);
			}

			if(tSec < 10)
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"0%d",tSec);
			} else
			{
				sprintf(&TaskBarText[strlen(TaskBarText)],"%d",tSec);
			}


			SetWindowText(hWnd, TaskBarText); // Update Text

			Sleep(10); // Avoid using 100% cpu
		
	}

	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) //Decide what to do
	{
	case DLL_PROCESS_ATTACH: //On dll attach
		{
			

			string CmdArgs = GetCommandLineA(); //Get the commandline
			int pos = CmdArgs.find("-camfile:"); //Try to look for the camfile path
			if (pos != string::npos) //If found:
				strcpy(camFullPath, CmdArgs.substr(pos + 9).c_str()); //pos points to beginning of -camfile: string, so it's +9 chars

			hMainThread = CreateThread(NULL, 0, MainThread, 0, 0, 0); // Create Main Loop
			hWindowUpdate = CreateThread(NULL, 0, WindowUpdate, 0, 0, 0); // Create Window update loop
			hUserInteraction = CreateThread(NULL, 0, UserInteraction, 0, 0, 0); // Create message recording thread
		}
		break;
	case DLL_THREAD_ATTACH: //On thread attach
		break;
	case DLL_THREAD_DETACH: //On thread detach
		break;
	case DLL_PROCESS_DETACH: //on process detach
		break;
	}
	return TRUE;
}

