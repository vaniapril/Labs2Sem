#pragma once

#include "stdafx.h"

class Application
{
	LPCWSTR _windowTitle;
	LPCWSTR _szWindowClass;
	HINSTANCE _hInst;
	
public:
	Application(LPCWSTR windowTitle): _windowTitle(windowTitle)
	{
		_hInst = nullptr;
		_szWindowClass = TEXT("MainWindow");
	}

	int APIENTRY Run(
		_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);

	ATOM MyRegisterClass(HINSTANCE hInstance);

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
};
