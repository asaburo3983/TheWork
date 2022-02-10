#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include "Window.h"
#include "DirectGraphics.h"

#pragma comment(lib, "winmm.lib")


#include "DXTK11Lib.h"
#include "input.h"

#include "SaveLoadMap.h"

#include<Shlwapi.h>

#include "MainGame.h"



using namespace DXTK11LIB;
using namespace GameMath;

BOOL WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits)
{
	HANDLE           hFile;
	DWORD            dwResult;
	DWORD            dwSizeImage;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

	hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	dwSizeImage = nHeight * ((3 * nWidth + 3) / 4) * 4;

	ZeroMemory(&bmfHeader, sizeof(BITMAPFILEHEADER));
	WCHAR abc[] = L"BM";
	bmfHeader.bfType = *(LPWORD)"BM";
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSizeImage;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwResult, NULL);

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nWidth;
	bmiHeader.biHeight = nHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biSizeImage = dwSizeImage;
	bmiHeader.biCompression = BI_RGB;

	WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwResult, NULL);

	WriteFile(hFile, lpBits, dwSizeImage, &dwResult, NULL);

	CloseHandle(hFile);

	return TRUE;
}

HBITMAP CreateBackbuffer(int nWidth, int nHeight)
{
	LPVOID           lp;
	BITMAPINFO       bmi;
	BITMAPINFOHEADER bmiHeader;

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nWidth;
	bmiHeader.biHeight = nHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;

	bmi.bmiHeader = bmiHeader;

	return CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS, &lp, NULL, 0);
}


bool clear = false;
float clearTime = 0;
void SetClear() {
	clear = true;
	clearTime = 0;
}
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{

	int wx = GetSystemMetrics(SM_CXSCREEN);
	
	int wy = GetSystemMetrics(SM_CYSCREEN);

	Window window("まどでる！", wx, wy);

	SetDefaultImageExtend((float)wx/1920.0);
	GameStatic::GetInstance()->mouseExtend = 1920.0 / (float)wx;

	// ウィンドウを作成
	if (window.Create() == false)
	{
		//return 0;
	}

	//キー入力初期化
	InitInput(hInstance, FindWindow(Window::ClassName, nullptr));

	DirectGraphics::GetInstance()->Init();

	InitDXTK11Lib();

	
	////画面のキャプチャ
	HDC     hdc;
	HWND    hwndDesk;
	RECT    rc;
	BITMAP  bm;
	HBITMAP hbmp;
	HBITMAP hbmpPrev;

	hwndDesk = GetDesktopWindow();
	GetWindowRect(hwndDesk, &rc);

	hdc = CreateCompatibleDC(NULL);
	hbmp = CreateBackbuffer(rc.right, rc.bottom);
	hbmpPrev = (HBITMAP)SelectObject(hdc, hbmp);

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, GetWindowDC(hwndDesk), 0, 0, SRCCOPY);

	GetObject(hbmp, sizeof(BITMAP), &bm);
	TCHAR	pszSrc[] = "Data/capture.bmp";

	WriteBitmap(pszSrc, rc.right, rc.bottom, bm.bmBits);

	SelectObject(hdc, hbmpPrev);
	DeleteObject(hbmp);
	DeleteDC(hdc);
	//////////////

	Image mainBack;
	mainBack.Load("Data/capture.bmp");

	MainGameLoad();

	int startTime = 0;
	
	const int maxX = 64;
	const int maxY = 36;
	int dist = 1920 / maxX;

	Pos2 posBack[maxY][maxX];
	Pos2 posBackFirstData[maxY][maxX];
	int speedBack[maxY][maxX];
	for (int y = 0; y < maxY; y++) {

		for (int x = 0; x < maxX; x++) {

			posBack[y][x].p[0] = 960 + dist * x;
			posBack[y][x].p[1] = 540 + dist * y;
			posBackFirstData[y][x] = posBack[y][x];

			speedBack[y][x] = GetRand(5, 30);
		}
	}

	Image startText;
	startText.Load("Data/Start.png");
	int startTextY = 540;
	while (true)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{

				UpdateInput();//入力処理更新
			//描画前処理
			DirectGraphics::GetInstance()->StartRendering();
			DirectGraphics::GetInstance()->SetUpContext(nullptr, nullptr);
			GameStatic* gs = GameStatic::GetInstance();

			
			startTime++;
			mainBack.DrawEX(960,540, 0, 1, 1, 0.75, 1, true, false);

			
			if (clear) {
				gs->MOUSE->left = 0;
			}


	
			MainGameUpdata();

		
			MainGameDraw();

			if (clear == false) {
				for (int y = 0; y < maxY; y++) {

					for (int x = 0; x < maxX; x++) {
						if (startTime >= 180 && startTime < 1000 && gs->scene != GAME_CLEAR) {
							posBack[y][x].p[1] += speedBack[y][x];
						}
						else if (gs->scene == GAME_CLEAR) {
							if (posBack[y][x].p[1] > posBackFirstData[y][x].p[1])
								posBack[y][x].p[1] -= speedBack[y][x] * 2;
							else {
								posBack[y][x].p[1] = posBackFirstData[y][x].p[1];
							}
						}
						if (gs->scene != GAME) {
							mainBack.DrawEX(posBack[y][x].p[0], posBack[y][x].p[1], 0, 1, 1, 1, 1, false, false, RECT{ 0 + x * dist,0 + y * dist,dist + x * dist,dist + y * dist });
						}
					}
				}
			}

			if (clear) {
				clearTime += 0.01;

				mainBack.Draw(960,540,0,1,clearTime-1);

				if (clearTime > 2.0f) {
					break;
				}
			}


			if (startTime % 90 < 45) {
				startText.Draw(960, startTextY);
			}
			if (startTextY < 2000&&startTime >= 180) {
				startTextY += 10;
			}
			//DrawFPS();
			DirectGraphics::GetInstance()->FinishRendering();//描画する毎回の処理

			if (GameStatic::GetInstance()->KEY->key[DIK_ESCAPE]) {
				break;
			}


		}
	}

	UninitInput();
	
	DirectGraphics::GetInstance()->Release();

	return 0;
}
