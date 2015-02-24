#include <windows.h>
#include <tchar.h>
#include <time.h>
#include "resource.h"
#define work 3
// 1 just off the walls							//1 ������ �� ���� 
// 2 from one static tskok // not quite right	//2 �� ������ ������������ ����� // �������� ��������
// 3 rebound all + works acceptably				//3 �� ���� ������ // + �������� ���������
// 4 all bounce backwards						//4 �� ���� ������ � �������� ������� 

UINT countWindow = 10U, currentCountWindow = 0U;

unsigned int minWindSize = 30U, maxWindSize = 150U; 


struct Windows
{
	HWND discriptor;
	UINT direction;
	UINT width;
	UINT height; 
	int X;
	int Y;
	Windows()
	{
		discriptor=0;
		width = rand() % (maxWindSize - minWindSize) + minWindSize;
		height = rand() % (maxWindSize - minWindSize) + minWindSize;
		direction = rand() % 4;
		X = 200;
		Y = 200;
	}
};

Windows* ptr = nullptr;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	srand(unsigned(time(0)));
	ptr = new Windows[countWindow];

	wchar_t ac[100] = {};
	swprintf(ac, 100, TEXT("Open Calc.exe min=1, max=%d, and Press OK to start"), countWindow);
	MessageBox(0, ac, TEXT("Game"), MB_OK | MB_ICONASTERISK);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

HWND serch12 = 0;
BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{	
	static bool Timer = false;
	static bool was = false;
	static unsigned int time = 20U;
	static unsigned int countTime = 0U;

	unsigned int Xmax = GetSystemMetrics(SM_CXSCREEN);	
	unsigned int Ymax = GetSystemMetrics(SM_CYSCREEN);	

	static unsigned int move = 15U;
	static unsigned int countCalc = 1U;
	wchar_t find[150] = {};

	HWND serch = nullptr;
	RECT myrect, nextRect; //myrect - next, nextRect - privious

	switch (message)
	{
	case WM_TIMER:

		if (countWindow!=currentCountWindow) // serch opened calc.exe
		{
			for (unsigned int i = 0U; i < countWindow; ++i)
			{
				
				if (
					(serch = FindWindow(TEXT("CalcFrame"), TEXT("�����������")))	// for russian
					||
					(serch = FindWindow(TEXT("CalcFrame"), TEXT("Calculator")))		 // for english
					)
				{
#if work ==2												
					static bool tr = true;
					if (!currentCountWindow && tr)
					{
						serch12 = serch;
						tr = false;
						SetWindowText(serch12, TEXT("serch12"));
					}
					else
					{			
						ptr[currentCountWindow++].discriptor = serch;
						swprintf(find, 20, TEXT("calc%d"), countCalc++);
						SetWindowText(serch, find);
					}								
#else 			
					ptr[currentCountWindow++].discriptor = serch;
					swprintf(find, 20, TEXT("calc%d"), countCalc++);
					SetWindowText(serch, find);
#endif 
				}
			}
		}

#if work == 2
		for (unsigned int i = 0; i < currentCountWindow; ++i)
		{
			GetWindowRect(ptr[i].discriptor, &myrect);
			GetWindowRect(serch12, &nextRect);

			swprintf(find, 150, TEXT(" top=%d,  bottom=%d, right=%d, left=%d"), 
				nextRect.top,
				nextRect.bottom,
				nextRect.right,
				nextRect.left
				);
			SetWindowText(hWnd, find);

			//c2 ���� ������ ����� �����
			if (
				(nextRect.top < myrect.bottom && myrect.bottom < nextRect.bottom)  
				&&
				(nextRect.right > myrect.left && myrect.left > nextRect.left)
				)
			{
				UINT random = rand() % 2;
				ptr[i].direction = (random ? 1U : 3U);
			}
			// �1 ���� ������ ������ �����
			if (
				(nextRect.top < myrect.bottom && myrect.bottom < nextRect.bottom)
				&&
				(nextRect.right > myrect.right && myrect.right > nextRect.left)
				)
			{
				UINT random = rand() % 2;
				ptr[i].direction = (random ? 1U : 3U);
			}
			// �4 ���� ������� ������ �����
			if (
				(nextRect.top < myrect.top && myrect.top < nextRect.bottom)
				&&
				(nextRect.right > myrect.right && myrect.right > nextRect.left)
				)
			{
				UINT random = rand() % 2;
				ptr[i].direction = (random ? 2U : 0U);
			}
			// �3 ���� ������� ����� �����
			if (
				(nextRect.top < myrect.top && myrect.top < nextRect.bottom)
				&&
				(nextRect.right > myrect.left && myrect.left > nextRect.left)
				)
			{
				UINT random = rand() % 2;
				ptr[i].direction = (random ? 2U : 0U);
			}
		}

#elif work == 3			
		for (unsigned int i = 0; i < currentCountWindow; ++i)
		{
			for (unsigned int j = 0; j < currentCountWindow; ++j)
			{
				if (i != j) 
				{
					GetWindowRect(ptr[i].discriptor, &myrect);
					GetWindowRect(ptr[j].discriptor, &nextRect);

					//hit the lower left corner //c2 ���� ������ ����� ����� 
					if (
						(nextRect.top < myrect.bottom && myrect.bottom < nextRect.bottom)
						&&
						(nextRect.right > myrect.left && myrect.left > nextRect.left)
						)
					{
						UINT random = rand() % 2;
						ptr[i].direction = (random ? 1U : 3U);
						ptr[j].direction = (random ? 2U : 0U);
					}
					//hit the lower right corner // �1 ���� ������ ������ �����
					if (
						(nextRect.top < myrect.bottom && myrect.bottom < nextRect.bottom)
						&&
						(nextRect.right > myrect.right && myrect.right > nextRect.left)
						)
					{
						UINT random = rand() % 2;
						ptr[i].direction = (random ? 1U : 3U);
						ptr[j].direction = (random ? 2U : 0U);
					}
					// hit the top right corner // �4 ���� ������� ������ �����
					if (
						(nextRect.top < myrect.top && myrect.top < nextRect.bottom)
						&&
						(nextRect.right > myrect.right && myrect.right > nextRect.left)
						)
					{
						UINT random = rand() % 2;
						ptr[i].direction = (random ? 2U : 0U);
						ptr[j].direction = (random ? 1U : 3U);
					}
					// hit the upper left corner // �3 ���� ������� ����� �����
					if (
						(nextRect.top < myrect.top && myrect.top < nextRect.bottom)
						&&
						(nextRect.right > myrect.left && myrect.left > nextRect.left)
						)
					{
						UINT random = rand() % 2;
						ptr[i].direction = (random ? 2U : 0U);
						ptr[j].direction = (random ? 1U : 3U);
					}
				}
			}
		}
#elif work == 4
		for (unsigned int i = 0; i < currentCountWindow; ++i)
		{
			for (unsigned int j = 0; j < currentCountWindow; ++j)
			{
				if (i != j) //+++++
				{
					GetWindowRect(ptr[i].discriptor, &myrect);
					GetWindowRect(ptr[j].discriptor, &nextRect);

					bool flag = false;
					//c2 ���� ������ ����� �����
					if (
						(nextRect.top < myrect.bottom && myrect.bottom < nextRect.bottom)
						&&
						(nextRect.right > myrect.left && myrect.left > nextRect.left)
						)
					{
						flag = true;
					}
					// �1 ���� ������ ������ �����
					if (
						(nextRect.top < myrect.bottom && myrect.bottom < nextRect.bottom)
						&&
						(nextRect.right > myrect.right && myrect.right > nextRect.left)
						)
					{
						flag = true;
					}
					// �4 ���� ������� ������ �����
					if (
						(nextRect.top < myrect.top && myrect.top < nextRect.bottom)
						&&
						(nextRect.right > myrect.right && myrect.right > nextRect.left)
						)
					{
						flag = true;
					}
					// �3 ���� ������� ����� �����
					if (
						(nextRect.top < myrect.top && myrect.top < nextRect.bottom)
						&&
						(nextRect.right > myrect.left && myrect.left > nextRect.left)
						)
					{
						flag = true;
					}

					if (flag)
					{
						if (ptr[i].direction == 0)
						{
							ptr[i].direction = 1;
							ptr[j].direction = 0;
						}
						else if (ptr[i].direction == 1)
						{
							ptr[i].direction = 0;
							ptr[j].direction = 1;
						}
						else if (ptr[i].direction == 2)
						{
							ptr[i].direction = 3;
							ptr[j].direction = 2;
						}
						else if (ptr[i].direction == 3)
						{
							ptr[i].direction = 2;
							ptr[j].direction = 3;
						}
					}
					}
				}
			}
#endif
		
		for (unsigned int i = 0; i < currentCountWindow; ++i)
		{
			GetWindowRect(ptr[i].discriptor, &myrect);		
			
			//move and chek walls
			if (ptr[i].direction == 0)// Right down // ������ ����
			{
				ptr[i].X += move;
				ptr[i].Y += move;

				if (myrect.right >= Xmax)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 2U : 1U);
				}
				if (myrect.bottom >= Ymax)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 3U : 1U); 
				}
			}
			else if (ptr[i].direction == 1)// Left up // ����� �����
			{
				ptr[i].X -= move;
				ptr[i].Y -= move;

				if (myrect.top <= 0)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 2U : 0U);  
				}
				if (myrect.left <= 0)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 3U : 0U); 
				}
			}
			else if (ptr[i].direction == 2)// Left Down //����� ���� 
			{
				ptr[i].X -= move;
				ptr[i].Y += move;

				if (myrect.left <= 0)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 3U : 0U); 

				}
				if (myrect.bottom >= Ymax)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 3U : 1U); 
				}
			}
			else if (ptr[i].direction == 3)// Right up // ������ ����� 
			{
				ptr[i].X += move;
				ptr[i].Y -= move;

				if (myrect.top <= 0)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 2U : 0U);
				}
				if (myrect.right >= Xmax)
				{
					UINT random = rand() % 2;
					ptr[i].direction = (random ? 2U : 1U);
				}
			}

			MoveWindow(ptr[i].discriptor,	// ���������� ����
				ptr[i].X,					// �������������� ���������
				ptr[i].Y,					// ������������ ���������
				ptr[i].width,				// ������
				ptr[i].height,				// ������
				true						// ���� �����������
				);
				
		}
		return TRUE;

	case WM_INITDIALOG:
		// not work ++++ begin
		//case WM_KEYDOWN: // ��� ������� ��� ������� �� enter  //if (wp == VK_RETURN && !was)
		if (WM_KEYDOWN) // ������ �� ��������+++++++++++++
		{
			if (VK_CONTROL) //&& !Timer)
			{
				Timer = true;
				SetTimer(hWnd, 1, time, NULL);
			}
			if (VK_SHIFT) //&& Timer)
			{
				Timer = false;
				KillTimer(hWnd, 1);
			}
		}
		// not work ++++ end
		if (!was)
		{
			ShowWindow(hWnd, SW_SHOWMINIMIZED);

			was = true;
			Timer = true;
			SetTimer(hWnd, 1, time, NULL);	// ��������� ������� �� ������� ������� <ENTER> not work
			
			//ShellExecute(0, TEXT("open"), TEXT("calc.exe"), 0, 0, SW_SHOW);
			//ShellExecute(0, TEXT("open"), TEXT("notepad.exe"), 0, 0, SW_SHOW);

			//ptrNotepad[i].discriptor = FindWindow(TEXT("CalcFrame"), TEXT("�����������")); // ��� ������. // ��������� ����.
			//ptrNotepad[i].discriptor = FindWindow(TEXT("Notepad"), TEXT("����������-�������"));//TEXT("���������� - �������") GetForegroundWindow();
			
		}
		return TRUE;

	case WM_CLOSE: // �� WM_DESTROY!
		KillTimer(hWnd, 1);
		EndDialog(hWnd, 0); 
		delete ptr;
		return TRUE;
	}
	return FALSE;
}