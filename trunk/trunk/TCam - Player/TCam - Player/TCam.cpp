#include "TCam.h"
#include "resource.h"

CNetworkClient NetworkClientCharList; // Used to establish connection with Login Server
CNetworkClient NetworkClient; // Used to Game Server emulator
XTEA Xtea; // Encryption/decryption
CRegistry Registry;
CTCamReader Cam;

DWORD WINAPI MainThread(LPVOID lpParam)
{

	while(!(hWnd = FindWindow("TibiaClient", 0))) // Get window handle
		Sleep(1); 
	wndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)TibiaHwNd); // Set message pointer

	while(!IsWindowVisible(hWnd)) // Loop until the window is visible
		Sleep(1);

	Sleep(1000);

	NetworkClientCharList.Setup(7151,true);
	NetworkClient.Setup(7152,false);

	if(!NetworkClientCharList.proxyReady) // Check if everything is set up
	{
		MessageBoxA(MainDialog,"Failed to initialize proxy.","TCam Player",MB_OK);
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

	unsigned char Packet[512]; // Create our own login packet
	memset(Packet,0,512);
	int BufferLen = CreateCharList(Packet,"Movie");

	Sleep(100);

	Xtea.EncryptPacket((unsigned char *)Packet); // Encrypt it
	NetworkClientCharList.SendMessageClient((char *)Packet,BufferLen+2); // Send it to the client

	if(!NetworkClient.proxyReady) // Check if everyhtings OK
	{
		MessageBoxA(MainDialog,"Failed to initialize proxy.","TCam Player",MB_OK);
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

	DWORD Len = 260; 
	Registry.QueryStringValue(HKEY_LOCAL_MACHINE,"Software\\TibiaFreak\\TCam", "CamPlaying",camFullPath,Len); // Get cam full-path

	// Start reading file and playing cam
	Cam.Open(camFullPath);

	int myCamTime = (Cam.TotalPlayTime / 1000); // Calcualte total running time
	tSec = myCamTime % 60;
	tMin = (myCamTime / 60) % 60;
	tHour = (myCamTime / 60) / 60;

	while(1)
	{
		if(clock() > (initClock + (waitTime/speed))) // Delay tunil next packet
		{
			CamPacket myPacket = Cam.ReadNextPacket(); // Read next packet
			if(myPacket.id == 0x30) // If XTEA update
			{
			} else if(myPacket.id == 0x33)
			{
			} else
			{
				Playing = true;
				NetworkClient.SendMessageClient(myPacket.buf,myPacket.nSize); // Send packet to client

				initClock = clock();
				waitTime = (int)((double)myPacket.delay); // Delay
				lastWait = waitTime;
			}
		}
		if(!Cam.reset)
		{
			Sleep(1);
		} else
		{
			while(Cam.reset)
			{
				CamPacket resetPacket = Cam.ReadNextPacket(); // Read next packet

				if(resetPacket.id == 0x30) // If XTEA update
				{
				} else 
				{
					NetworkClient.SendMessageClient(resetPacket.buf,resetPacket.nSize); // Send packet to client
				}
			}
			
		}
	}

	return 0;
}

int CreateCharList(unsigned char *buffer, char *name) // Should be self explainatory
{
	int nLen = strlen(name);
	int x = 27 + nLen;
	while(x % 8 != 0)
	{
		x++;
	}
	buffer[0] = x;
	buffer[1] = 0x00;
	buffer[2] = 27 + nLen;
	buffer[3] = 0x00;
	buffer[4] = 0x14;
	buffer[5] = 0x06;
	buffer[6] = 0x00;
	buffer[7] = 0x33;
	buffer[8] = 0x33;
	buffer[9] = 0x33;
	buffer[10] = 0x4F;
	buffer[11] = 0x6D;
	buffer[12] = 0x67;
	buffer[13] = 0x64;
	buffer[14] = 0x01;
	buffer[15] = nLen & 0xFF;
	buffer[16] = 0x00;
	memcpy(&buffer[17], name, nLen);
	buffer[17+nLen] = 4 & 0xFF;
	buffer[18+nLen] = 0x00;
	memcpy(&buffer[19+nLen], "TCam", 4);
	buffer[19+nLen+4] = (char)127;
	buffer[20+nLen+4] = (char)0;
	buffer[21+nLen+4] = (char)0;
	buffer[22+nLen+4] = (char)1;
	buffer[23+nLen+4] = 0xF0; // 7152
	buffer[24+nLen+4] = 0x1B;
	buffer[25+nLen+4] = 0xFF;
	buffer[26+nLen+4] = 0xFF;

	return x;
}

LRESULT APIENTRY TibiaHwNd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // Hook to tibias message handler
{
	if(!Cam.reset)
	{
		switch(uMsg)
		{

		case WM_KEYDOWN:
			{
				if(wParam == VK_UP) // up arrow pressed
				{
					speed = 50.0;
					wParam = NULL;
				}
				else if(wParam == VK_LEFT) // left
				{
					if(speed <= 1.0)
					{
						if(speed != 0.1)
							speed -= 0.1;
					}
					else
					{ 
						speed -= 1.0;
					}
					wParam = NULL;
				}
				else if(wParam == VK_DOWN) // down
				{
					speed = 1.00;
					wParam = NULL;
				}
				else if(wParam == VK_RIGHT) // right
				{
					if(speed >= 50.0)
					{
						speed = 50.0;
					} 
					else if(speed >= 1.0)
					{
						speed += 1.0;
					}
					else
					{
						speed += 0.1;
					}
					wParam = NULL;
				} 
				else if(wParam == VK_BACK)
				{
					speed = 1.0;
					Cam.Reset(Cam.CurrentPlayTime - 30 * 1000);
				}
				else if(wParam == VK_DELETE)
				{
					Sleep(1000);

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
	}
	return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam ); // return the message
} 

DWORD WINAPI UserInteraction(LPVOID lpParam)
{
	while(1)
	{
		if(!Cam.reset)
		{
			int SkipTo = NetworkClient.RecMessageClient();

			if(SkipTo > 0 && SkipTo < Cam.TotalPlayTime)
			{
				speed = 1.0;
				Sleep(100);
				Cam.Reset(SkipTo);
			} 
			Sleep(1);
		}
	}
}

DWORD WINAPI WindowUpdate(LPVOID lpParam)
{
	while(1)
	{
		if(!Cam.reset)
		{
			if(speed < 0.1)
				speed = 0.1;

			cSec = (Cam.CurrentPlayTime / 1000) % 60;
			cMin = ((Cam.CurrentPlayTime / 1000) / 60) % 60;
			cHour = ((Cam.CurrentPlayTime / 1000) / 60) / 60;

			if(speed < 0.1)
				speed = 0.1;

			char TaskBarText[100];
			sprintf(&TaskBarText[0],"TCam - %.1fx - ",speed); // Format time

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

			Sleep(1); // Avoid using 100% cpu
		}
	}

	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) //Decide what to do
	{
	case DLL_PROCESS_ATTACH: //On dll attach
		{
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
