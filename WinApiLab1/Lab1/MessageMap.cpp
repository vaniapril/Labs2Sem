#include "stdafx.h"
#include "MessageMap.h"

std::map<int, LRESULT(*)(HWND, UINT, WPARAM, LPARAM)> MessageMap;