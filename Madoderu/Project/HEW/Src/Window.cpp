﻿#include "Window.h"
#include "resource.h"
const char *Window::ClassName = "DX11PracticeWindow";

LRESULT CALLBACK Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

bool Window::Create()
{
	if (EntryWindowClass() == false)
	{
		return false;
	}

	// ウィンドウ作成
	HWND window_handle = CreateWindowEx(
		//ウィンドウ背景透過
		//WS_EX_LAYERED,
		NULL,
		Window::ClassName,
		m_Title,

		//通常
		//(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
		//フルスクリーン
		WS_MAXIMIZE | WS_POPUP,
		CW_USEDEFAULT,
		0,
		m_Width,
		m_Height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	//Windows背景の透過
	//SetLayeredWindowAttributes(window_handle, RGB(0, 255, 0), 0, LWA_COLORKEY);
	SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));

	
	if (window_handle == NULL)
	{
		return false;
	}

	ResizeWindow(window_handle);

	return true;
}

bool Window::EntryWindowClass()
{
	WNDCLASSEXA window_class = {
		sizeof(WNDCLASSEX),				// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
		WindowProc,						// ウィンドウプロシージャ
		0,								// 補助メモリ
		0,								// 補助メモリ
		GetModuleHandle(NULL),			// このプログラムのインスタンスハンドル
		LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1), //アイコン
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,							// 背景ブラシ(背景色)
		NULL,							// メニュー名
		Window::ClassName,				// クラス名									
		NULL							// 小さいアイコン
	};

	// 構造体の登録
	if (RegisterClassExA(&window_class) == 0)
	{
		return false;
	}

	return true;
}

void Window::ResizeWindow(HWND window_handle)
{
	RECT window_rect;
	RECT client_rect;

	GetWindowRect(window_handle, &window_rect);
	GetClientRect(window_handle, &client_rect);

	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	SetWindowPos(window_handle, NULL, CW_USEDEFAULT, 0, frame_size_x + m_Width, frame_size_y + m_Height, SWP_NOMOVE);

	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);
}
