#pragma once

#include "stdafx.h"
#include <map>

extern std::map<int, LRESULT(*)(HWND, UINT, WPARAM, LPARAM)> MessageMap;