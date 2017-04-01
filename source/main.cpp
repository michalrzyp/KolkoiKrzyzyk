#include<Windows.h>
#include<stdio.h>
#include "res.h"

enum wynikGry
{
	wygrywaKolko,
	wygrywaKrzyzyk,
	remis,
	nierozstrzygniety
};
bool isGameOn = false;
bool isFirstPlayerTurn = true;
CHAR Text[200];
int Array[9];// 0 puste  pole 1 pole X -1 pole O
void drawX(HDC hdc,int x, int y);
void drawO(HDC hdc, int x, int y);
void drawBoard(HDC hdc);
void Result(HWND hwndDig, wynikGry a);
wynikGry CheckResult();
wynikGry a = nierozstrzygniety;
INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON11:
				if (isGameOn == false)
				{
					isGameOn = true;
					HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
					wsprintf(Text, "Gra sie zaczyna");
					SetWindowText(hwndStatic2, Text);
					for (int i = 0; i < 9; i++)
					{
						Array[i] = 0;
					}
					HWND hwndButton11 = GetDlgItem(hwndDig, IDC_BUTTON11);
					wsprintf(Text, "Stop");
					SetWindowText(hwndButton11, Text);
					CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1) == 0 ? 0 : 
					CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
					HDC hdc = GetDC(hwndDig);//rysowanie siatki gry
					//LineTo
					//MoveToEx
					//Rectange
					//TextOut
					//GetPixel
					//SetPixel
					HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 192, 203));
					SelectObject(hdc, hMyPen);
					//DeleteObject(hMyPen);
					//TextOut(hdc, 0, 0, Text, strlen(Text));
					drawBoard(hdc);
					ReleaseDC(hwndDig, hdc);
				}
				else
				{
					isGameOn = false;
					HWND hwndButton11 = GetDlgItem(hwndDig, IDC_BUTTON11);
					wsprintf(Text, "Start");
					SetWindowText(hwndButton11, Text);
					HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
					wsprintf(Text, "Gra sie konczy");
					SetWindowText(hwndStatic2, Text);
				}
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	case WM_LBUTTONDOWN:
		if (isGameOn == true)
		{
			int x = LOWORD(lParam), y = HIWORD(lParam);
			if ((x >= 200 && x <= 410) && (y >= 50 && y <= 200))
			{
				int filedX = ((x - 200) / 70);
				int filedY = ((y - 50) / 50);
				if (Array[filedX * 3 + filedY] == 0)
				{
					x = filedX * 70 + 200;
					y = filedY * 50 + 50;//x=((x-iMinBoardX/iWidthBorardX)*iWidthX+iMinBoardX+iWidthBoardX)
					HDC hdc = GetDC(hwndDig);
					HWND hCtl1 = GetDlgItem(hwndDig, IDC_RADIO1);
					HWND hCtl2 = GetDlgItem(hwndDig, IDC_RADIO2);
					if (isFirstPlayerTurn == true)
					{
						drawX(hdc, x, y);
						Array[3 * filedX + filedY] = 1;
						CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
						EnableWindow(hCtl1, FALSE);
						EnableWindow(hCtl2, TRUE);
					}
					else
					{
						drawO(hdc, x, y);
						Array[3 * filedX + filedY] = -1;
						CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
						EnableWindow(hCtl2, FALSE);
						EnableWindow(hCtl1, TRUE);
					}
					ReleaseDC(hwndDig, hdc);
					isFirstPlayerTurn = !isFirstPlayerTurn;
				}
			}
		}
		a = CheckResult();
		Result(hwndDig, a);
		return TRUE;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwndDig, &ps);
		HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 192, 203));
		SelectObject(hdc, hMyPen);
		drawBoard(hdc);
		//DrawallBoards(hdc)
		int x, y;
		for (int i = 0; i < 9; i++)
		{
			switch (i / 3)
			{
			case 0: x = 200; break;
			case 1: x = 270; break;
			case 2: x = 340; break;
			}
			switch (i % 3)
			{
			case 0: y = 50; break;
			case 1: y = 100; break;
			case 2: y = 150; break;
			}
			if (Array[i] == 0)
				Array[i] == 1 ? drawX(hdc, x, y) : drawO(hdc, x, y);
		}
		ReleaseDC(hwndDig, hdc);
		EndPaint(hwndDig, &ps);
	}
	return TRUE;
	case WM_CLOSE:
		DestroyWindow(hwndDig);
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;
}
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLIne, int iCmdShow)
{
	HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	ShowWindow(hwndMainWindow, iCmdShow);
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
void drawX(HDC hdc, int x, int y)
{
	x += 35;
	y += 25;
	MoveToEx(hdc, x - 10, y - 10, NULL);
	LineTo(hdc, x + 10, y + 10);
	MoveToEx(hdc, x - 10, y + 10, NULL);
	LineTo(hdc, x + 10, y - 10);
}
void drawO(HDC hdc, int x, int y)
{
	x += 35;
	y += 25;
	MoveToEx(hdc, x - 10, y - 10, NULL);
	LineTo(hdc, x - 10, y + 10);
	LineTo(hdc, x - 10, y - 10);
	LineTo(hdc, x + 10, y - 10);
	LineTo(hdc, x + 10, y + 10);
	LineTo(hdc, x - 10, y + 10);
}
void drawBoard(HDC hdc)
{
	MoveToEx(hdc, 200, 100, NULL);
	LineTo(hdc, 410, 100);
	MoveToEx(hdc, 200, 150, NULL);
	LineTo(hdc, 410, 150);

	MoveToEx(hdc, 270, 50, NULL);
	LineTo(hdc, 270, 200);
	MoveToEx(hdc, 340, 50, NULL);
	LineTo(hdc, 340, 200);
}

wynikGry CheckResult()
{
	for (int i = 0; i < 3; i++)
	{
		if (Array[3 * i] == Array[3 * i + 1] && Array[3 * i] == Array[3 * i + 2])
		{
			if (Array[3 * i] == 1)
				return wygrywaKrzyzyk;
			if (Array[3 * i] == -1)
				return wygrywaKolko;
		}
		if (Array[i] == Array[i + 3] && Array[i] == Array[i + 6])
		{
			if (Array[i] == 1)
				return wygrywaKrzyzyk;
			if (Array[i] == -1)
				return wygrywaKolko;
		}
	}
	if ((Array[0] == Array[4] && Array[0] == Array[8]) || (Array[2] == Array[4] && Array[6] == Array[4]))
	{
		if (Array[4] == -1)
			return wygrywaKolko;
		if (Array[4] == 1)
			return wygrywaKrzyzyk;
	}
	for (int i = 0; i < 9; i++)
		if (Array[i] == 0)
			return nierozstrzygniety;
	return remis;
}

void Result(HWND hwndDig, wynikGry a)
{
	HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
	switch (a)
	{
	case wygrywaKolko:
		wsprintf(Text, "Wygrywa kolko");
		SetWindowText(hwndStatic2, Text);
		isGameOn = false;
		break;
	case wygrywaKrzyzyk:
		wsprintf(Text, "Wygrywa krzyzyk");
		SetWindowText(hwndStatic2, Text);
		isGameOn = false;
		break;
	case remis:
		wsprintf(Text, "Remis");
		SetWindowText(hwndStatic2, Text);
		isGameOn = false;
		break;
	case nierozstrzygniety:;
	default:;
	}
}