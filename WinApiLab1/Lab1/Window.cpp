#include "stdafx.h"
#include <string>
#include <vector>
#include "Colors.h"
#include "Application.h"
#include "MessageMap.h"
#include "TraficLightColor.h"

class Window
{
public:	
	static int _timerTicks;
	static int _timerMaxValue;

	static void InitializeMessageMap()
	{
		MessageMap[WM_PAINT] = OnPaint;
		MessageMap[WM_CREATE] = OnCreate;
		MessageMap[WM_DESTROY] = OnDestroy;
		MessageMap[WM_GETMINMAXINFO] = OnGetMinMaxInfo;
	}

private:
	static const int MinDialogWidth = 600;
	static const int MinDialogHeight = 450;
	static const int N = 8;
	static const int M = 2;
	static const int TimerID = 1;
	static const int TimerInterval = 1000;
	static const int N1 = 5;//green
	static const int N2 = 3;//yellow
	static const int N3 = 5;//red

	static LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		SetTimer(hWnd, TimerID, TimerInterval, TimerProc);
		_timerMaxValue = N1 + N2 + N3 + N2;
		return true;
	}

	static void CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR idEvent, DWORD time)
	{
		if (idEvent != TimerID) return;
		++_timerTicks;
		if (_timerTicks >= _timerMaxValue)
		{
			_timerTicks = 0;
		}
		InvalidateRect(hWnd, nullptr, true);
	}

	static LRESULT OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PostQuitMessage(0);
		return 0;
	}

	static LPARAM OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		auto BlackPen = CreatePen(PS_SOLID, 2, Colors::Black);
		auto WhiteBrush = CreateSolidBrush(Colors::White);

		RECT rect = _getClientRect(hWnd);
		RECT A1;
		_setRectFieldsByPoints(&A1, { rect.left, rect.top }, { rect.right / 2, rect.bottom / 2 });
		_cutEdge(&A1, 10, 5, 10, 5);
		DrawRect(hdc, A1, BlackPen, WhiteBrush);
		_cutEdge(&A1, 10, 5, 10, 5);
		RECT A2;
		_setRectFieldsByPoints(&A2, { rect.left, rect.bottom / 2 }, { rect.right / 2, rect.bottom });
		_cutEdge(&A2, 5, 10, 10, 5);
		DrawRect(hdc, A2, BlackPen, WhiteBrush);
		_cutEdge(&A2, 5, 10, 10, 5);
		RECT B;
		_setRectFieldsByPoints(&B, { rect.right / 2, rect.top }, { rect.right, rect.bottom});
		_cutEdge(&B, 10, 10, 5, 10);
		DrawRect(hdc, B, BlackPen, WhiteBrush);
		_cutEdge(&B, 10, 10, 5, 10);
		
		DrawInRectTask1(hdc, A1, 5, 1);
		DrawInRectTask1(hdc, A2, N, M);
		DrawInRectTask2(hdc, B);

		DeleteObject(BlackPen);
		DeleteObject(WhiteBrush);
		EndPaint(hWnd, &ps);
		return 0;
	}

	static LPARAM OnGetMinMaxInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto minMaxInfo = (LPMINMAXINFO)lParam;
		minMaxInfo->ptMinTrackSize = {MinDialogWidth, MinDialogHeight};
		return false;
	}

	static int DrawInRectTask1(HDC hdc, RECT& rect, int n, int m) {
		auto penBrush = LOGBRUSH();
		penBrush.lbColor = Colors::Red;
		auto RedDashPen = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 4, &penBrush, 0, nullptr);
		auto DarkBluePen = CreatePen(PS_SOLID, 2, Colors::DarkBlue);
		auto WhiteBrush = CreateSolidBrush(Colors::White);
		
		RECT square = _getSquare(rect);
		DrawEllipse(hdc, square, RedDashPen, WhiteBrush);
		int index = 0;
		double pi = _getPi();
		for (int i = 0; i < n; i++) {
			double a1 = pi * 2 / n * i + pi / 2;
			double a2 = pi * 2 / n * ((i + m) % n) + pi / 2;
			double r = 1;
			POINT p1 = _getPointInRect(square, _getXFromPolar(r, a1), _getYFromPolar(r, a1));
			POINT p2 = _getPointInRect(square, _getXFromPolar(r, a2), _getYFromPolar(r, a2));
			DrawLine(hdc,p1, p2,DarkBluePen);
		}
		
		DeleteObject(RedDashPen);
		DeleteObject(WhiteBrush);
		DeleteObject(DarkBluePen);
		return 0;
	}
	
	static int DrawInRectTask2(HDC hdc, RECT& rect) {
		HPEN GreenPen;
		HPEN YellowPen;
		HPEN RedPen;

		HBRUSH GreenBrush;
		HBRUSH YellowBrush;
		HBRUSH RedBrush;

		RECT R1;
		_setRectFieldsByPoints(&R1, _getPointInRect(rect, -1, 1), _getPointInRect(rect, 1, 1.0 / 3));
		R1 = _getSquare(R1);
		RECT R2;
		_setRectFieldsByPoints(&R2, _getPointInRect(rect, -1, 1.0 / 3), _getPointInRect(rect, 1, -1.0 / 3));
		R2 = _getSquare(R2);
		RECT R3;
		_setRectFieldsByPoints(&R3, _getPointInRect(rect, -1, - 1.0 / 3), _getPointInRect(rect, 1, -1));
		R3 = _getSquare(R3);

		int nowTime;
		TraficLight light = _getTraficLightColor(nowTime);
		switch (light) {
			case TraficLight::Red:
				GreenBrush = CreateSolidBrush(Colors::Green);
				YellowBrush = CreateSolidBrush(Colors::LightYellow);
				RedBrush = CreateSolidBrush(Colors::IndianRed);
				GreenPen = CreatePen(PS_SOLID, 5, Colors::Black);
				YellowPen = CreatePen(PS_SOLID, 2, Colors::Black);
				RedPen = CreatePen(PS_SOLID, 2, Colors::Black);
				_cutEdge(&R1, 5, 5, 5, 5);
				_cutEdge(&R2, 5, 5, 5, 5);
			break;
			case TraficLight::Yellow:
				nowTime = N1 + N2 + N3 - _timerTicks;
				GreenBrush = CreateSolidBrush(Colors::LightGreen);
				YellowBrush = CreateSolidBrush(Colors::LightYellow);
				RedBrush = CreateSolidBrush(Colors::Red);
				GreenPen = CreatePen(PS_SOLID, 2, Colors::Black);
				YellowPen = CreatePen(PS_SOLID, 2, Colors::Black);
				RedPen = CreatePen(PS_SOLID, 5, Colors::Black);
				_cutEdge(&R2, 5, 5, 5, 5);
				_cutEdge(&R3, 5, 5, 5, 5);
			break;
			case TraficLight::Green:
				GreenBrush = CreateSolidBrush(Colors::LightGreen);
				YellowBrush = CreateSolidBrush(Colors::Yellow);
				RedBrush = CreateSolidBrush(Colors::IndianRed);
				GreenPen = CreatePen(PS_SOLID, 2, Colors::Black);
				YellowPen = CreatePen(PS_SOLID, 5, Colors::Black);
				RedPen = CreatePen(PS_SOLID, 2, Colors::Black);
				_cutEdge(&R1, 5, 5, 5, 5);
				_cutEdge(&R3, 5, 5, 5, 5);
			break;
			default:
				GreenBrush = CreateSolidBrush(Colors::LightGreen);
				YellowBrush = CreateSolidBrush(Colors::Yellow);
				RedBrush = CreateSolidBrush(Colors::Red);
				GreenPen = CreatePen(PS_SOLID, 2, Colors::Black);
				YellowPen = CreatePen(PS_SOLID, 2, Colors::Black);
				RedPen = CreatePen(PS_SOLID, 2, Colors::Black);
		}

		auto text = std::to_wstring(nowTime);

		DrawEllipse(hdc, R1, RedPen, RedBrush);
		DrawEllipse(hdc, R2, YellowPen, YellowBrush);
		DrawEllipse(hdc, R3, GreenPen, GreenBrush);

		DrawTextInRect(hdc,R2,text,80,15);

		DeleteObject(GreenPen);
		DeleteObject(YellowPen);
		DeleteObject(RedPen);
		DeleteObject(GreenBrush);
		DeleteObject(YellowBrush);
		DeleteObject(RedBrush);
		return 0;
	}

	static TraficLight _getTraficLightColor(int& nowTime) {
		if (_timerTicks < N1)
		{
			nowTime = N1 - _timerTicks;
			return TraficLight::Red;
		} else if (_timerTicks >= N2 + N1 && _timerTicks < N1 + N2 + N3) {
			nowTime = N1 + N2 + N3 - _timerTicks;
			return TraficLight::Yellow;
		} else {
			if (_timerTicks < N2 + N1) {
				nowTime = N1 + N2 - _timerTicks;
			}
			else
			{
				nowTime = N1 + N2 + N3 + N2 - _timerTicks;
			}
			return TraficLight::Green;
		}
	}
	static double _getPi() {
		return atan(1) * 4;
	}
	// x,y from [-1,1]
	static POINT _getPointInRect(RECT& rect, double x, double y) {
		POINT center = _getCenterRoint(rect);
		return POINT{center.x + (LONG)(x * ((long long int)rect.right - rect.left) / 2), center.y - (LONG)(y * ((long long int)rect.bottom - rect.top) / 2)};
	}
	static RECT _getSquare(RECT& rect) {
		RECT square;
		POINT p1, p2;
		if (rect.right - rect.left >= rect.bottom - rect.top) {
			p1 = {(rect.right - (rect.bottom - rect.top) + rect.left) / 2, rect.top};
			p2 = {p1.x + (rect.bottom - rect.top), rect.bottom};
		}
		else
		{
			p1 = {rect.left, (rect.bottom - (rect.right - rect.left) + rect.top) / 2 };
			p2 = {rect.right, p1.y + (rect.right - rect.left)};
		}
		_setRectFieldsByPoints(&square, p1, p2);
		return square;
	}
	static POINT _getCenterRoint(RECT& rect)
	{
		return POINT{(rect.right - rect.left) / 2 + rect.left, (rect.bottom - rect.top) / 2 + rect.top};
	}
	static RECT _getClientRect(HWND& hwnd)
	{
		RECT clientRect;
		::GetClientRect(hwnd, &clientRect);
		return clientRect;
	}
	static double _getXFromPolar(double r, double a) {
		return r * cos(a);
	}
	static double _getYFromPolar(double r, double a) {
		return r * sin(a);
	}
	static void _setRectFieldsBySizes(RECT* rect, POINT p, int width, int height)
	{
		SetRect(rect, p.x, p.y, p.x + width, p.y + height);
	}
	static void _setRectFieldsByPoints(RECT* rect, POINT p1, POINT p2) {
		SetRect(rect, p1.x, p1.y, p2.x, p2.y);
	}
	static void _cutEdge(RECT* rect, int top, int bottom, int left, int right) {
		SetRect(rect, rect->left + left, rect->top + top, rect->right - right, rect->bottom - bottom);
	}

	static void DrawRect(HDC hdc, const RECT& rect, HPEN pen, HBRUSH brush)
	{
		auto oldBrush = SelectBrush(hdc, brush);
		auto oldPen = SelectPen(hdc, pen);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectBrush(hdc, oldBrush);
		SelectPen(hdc, oldPen);
	}
	static void DrawEllipse(HDC hdc, const RECT& rect, HPEN pen, HBRUSH brush) {
		auto oldBrush = SelectObject(hdc, brush);
		auto oldPen = SelectObject(hdc, pen);
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
	}
	static void DrawLine(HDC hdc, const POINT p1, POINT p2, HPEN pen) {
		auto oldPen = SelectPen(hdc, pen);
		POINT p;
		MoveToEx(hdc, p1.x,p1.y,&p);
		LineTo(hdc,p2.x,p2.y);
		SelectPen(hdc, oldPen);
	}
	static void DrawTextInRect(HDC hdc, RECT &rect, std::wstring text, int size, int weight)
	{
		auto lf = LOGFONT{};
		wcscpy_s(lf.lfFaceName, L"Lucida Console");
		//wcscpy_s(lf.lfFaceName,  L"Consolas");
		lf.lfHeight = size;
		lf.lfWeight = weight;
		lf.lfCharSet = DEFAULT_CHARSET;
		HFONT font = CreateFontIndirect(&lf);
		auto defaultFont = SelectObject(hdc, font);
		SetTextColor(hdc, Colors::Black);
		SetBkMode(hdc, TRANSPARENT);
		DrawTextW(hdc, text.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		auto old = SelectObject(hdc, defaultFont);
		DeleteObject(old);
	}
};

int Window::_timerTicks = 0;
int Window::_timerMaxValue = 0;

static int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window::InitializeMessageMap();
	Application(TEXT("Lab1")).Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}