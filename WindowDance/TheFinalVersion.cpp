#include <windows.h>
#include <tchar.h>
#include <time.h>
#include "resource.h"

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
		discriptor = 0;
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

		if (countWindow != currentCountWindow) // serch opened calc.exe
		{
			for (unsigned int i = 0U; i < countWindow; ++i)
			{
				if (
					(serch = FindWindow(TEXT("CalcFrame"), TEXT("Калькулятор")))	// for russian
					||
					(serch = FindWindow(TEXT("CalcFrame"), TEXT("Calculator")))		 // for english
					)
				{
					ptr[currentCountWindow++].discriptor = serch;
					swprintf(find, 20, TEXT("calc%d"), countCalc++);
					SetWindowText(serch, find);
				}
			}
		}

		for (unsigned int i = 0; i < currentCountWindow; ++i)
		{
			for (unsigned int j = 0; j < currentCountWindow; ++j)
			{
				if (i != j) 
				{
					GetWindowRect(ptr[i].discriptor, &myrect);
					GetWindowRect(ptr[j].discriptor, &nextRect);

					//hit the lower left corner //удар нижним левым углом 
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
					//hit the lower right corner //удар нижним правым углом
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
					// hit the top right corner //удар верхним правым углом
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
					// hit the upper left corner //удар верхним левым углом
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


			GetWindowRect(ptr[i].discriptor, &myrect);

			//move and chek walls
			if (ptr[i].direction == 0)// Right down // вправо вниз
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
			else if (ptr[i].direction == 1)// Left up // влево вверх
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
			else if (ptr[i].direction == 2)// Left Down //влево вниз 
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
			else if (ptr[i].direction == 3)// Right up // вправо вверх 
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

			MoveWindow(ptr[i].discriptor,	// дескриптор окна
				ptr[i].X,					// горизонтальное положение
				ptr[i].Y,					// вертикальное положение
				ptr[i].width,				// ширина
				ptr[i].height,				// высота
				true						// флаг перерисовки
				);

		}

		return TRUE;

	case WM_INITDIALOG:

		if (!was)
		{
			ShowWindow(hWnd, SW_SHOWMINIMIZED);

			was = true;
			Timer = true;
			SetTimer(hWnd, 1, time, NULL);	
		}
		return TRUE;

	case WM_CLOSE: 

		for (unsigned int i = 0; i < currentCountWindow; ++i)
		{
			SendMessage(ptr[i].discriptor, WM_CLOSE, 0, 0);
		}
		delete ptr;

		KillTimer(hWnd, 1);
		EndDialog(hWnd, 0);

		return TRUE;
	}
	return FALSE;
}