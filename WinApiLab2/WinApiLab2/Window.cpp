#include "stdafx.h"
#include "Colors.h"
#include "Application.h"
#include "MessageMap.h"
#include <string>


class Ball
{
	double _g;
	double _windage;

	double _X;
	double _Y;

	double _vX;
	double _vY;

	double _aX;
	double _aY;

	double _R;
	double _speedMultiplier;
	
public:
	Ball()
	{	
		_g = 9.81;
		_windage = 0.05;
		_speedMultiplier = 5;
		SetPosition(0, 0);
		_vX = 0;
		_vY = 0;
		_aX = 0;
		_aY = - _g;
		_R = 10;
	}
	
	double GetX() const
	{
		return _X;
	}
	double GetY() const
	{
		return _Y;
	}
	double GetVX() const
	{
		return _vX;
	}
	double GetVY() const
	{
		return _vY;
	}
	double GetAX() const
	{
		return _aX;
	}
	double GetAY() const
	{
		return _aY;
	}
	double GetR() const
	{
		return _R;
	}
	void SetPosition(double x, double y)
	{
		_X = x;
		_Y = y;
	}
	void SetSpeed(double vX, double vY) {
		_vX = vX / _speedMultiplier;
		_vY = vY / _speedMultiplier;
	}
	
	void Update(double dt)
	{
		dt *= _speedMultiplier;
		_X = _X + _vX * dt + _aX * dt * dt / 2;
		_Y = _Y + _vY * dt + _aY * dt * dt / 2;
		_vX += _aX * dt;
		_vY += _aY * dt;
		Windage(dt);
	}

	void Windage(double dt) {
		_vX -= _vX * dt * _windage;
		_vY -= _vY * dt * _windage;
	}

	void FlipX(double dx)
	{
		double d = _vX * _vX - 2.0 * _aX * dx;
		if (d < 0) {
			d = 0;
		}
		double sd = sqrt(d);
		double t;
		if (_aX != 0) {
			if (dx < 0) {
				t = (_vY + sd) / _aY;
			} else {
				t = (_vY - sd) / _aY;
			}
		} else {
			t = 0;
		}
		_X -= 2.0 * dx - _aX * t * t;
		_vX *= -1;
		_vX += 2.0 * _aX * t;
	}

	void FlipY(double dy)
	{	
		double d = _vY * _vY - 2.0 * _aY * dy;
		if (d < 0) {
			d = 0;
		}
		double sd = sqrt(d);
		double t;
		if (_aY != 0) {
			if(dy < 0){
				t = (_vY + sd) / _aY;
			} else {
				t = (_vY - sd) / _aY;
			}
		} else {
			t = 0;
		}
		_Y -= 2.0 * dy - _aY * t * t;
		_vY *= -1;
		_vY += 2.0 * _aY * t;
	}
};

class Room
{
public:
	double Left;
	double Right;
	double Top;
	double Bottom;

	double dyBottom;
	double dxLeft;
	double dyTop;
	double dxRight;

	bool isBlocked;
	Ball Ball;

	Room()
	{
		Left = -50;
		Right = +50;
		Bottom = 0;
		Top = 100;
		Ball.SetPosition(0, Top * 3 / 4);
		Ball.SetSpeed(50, 50);
		dyBottom = 0;
		dxLeft = 0;
		dyTop = 0;
		dxRight = 0;
		isBlocked = false;
	}

	void Update(double dt){
		if (!isBlocked) {
			_update(dt);
		}
	}

	void changeBlock() {
		isBlocked = !isBlocked;
	}
	void Block() {
		isBlocked = true;
	}
	void UnBlock() {
		isBlocked = false;
	}

	void _update(double dt) {
		Ball.Update(dt);
		dyBottom = Ball.GetY() - Ball.GetR() - Bottom;
		dxLeft = Ball.GetX() - Ball.GetR() - Left;
		dyTop = Ball.GetY() - Top + Ball.GetR();
		dxRight = Ball.GetX() - Right + Ball.GetR();

		if (dyBottom < 0) {
			Ball.FlipY(dyBottom);
		}
		if (dxLeft < 0) {
			Ball.FlipX(dxLeft);
		}
		if (dyTop > 0) {
			Ball.FlipY(dyTop);
		}
		if (dxRight > 0) {
			Ball.FlipX(dxRight);
		}
		if (Ball.GetY() - Ball.GetR() - Bottom < 0.0) {
			Ball.SetPosition(Ball.GetX(), Ball.GetR() + Bottom);
			Ball.SetSpeed(Ball.GetVX(), 0);
		}
		if (Ball.GetX() - Ball.GetR() - Left < 0.0) {
			Ball.SetPosition(Ball.GetR() + Left, Ball.GetY());
			Ball.SetSpeed(0, Ball.GetVY());
		}
		if (Ball.GetY() - Top + Ball.GetR() > 0.0) {
			Ball.SetPosition(Ball.GetX(), Top - Ball.GetR());
			Ball.SetSpeed(Ball.GetVX(), 0);
		}
		if (Ball.GetX() - Right + Ball.GetR() > 0.0) {
			Ball.SetPosition(Right - Ball.GetR(), Ball.GetY());
			Ball.SetSpeed(0, Ball.GetVY());
		}
	}
};

class Converter
{
	RECT _rect;
	double _startX;
	double _startY;
	double _endX;
	double _endY;

public:
	Converter() {
		_rect;
		_startX = 0;
		_startY = 0;
		_endX = 0;
		_endY=  0;
	}
	void UpdateRect(RECT rect, double startX, double startY, double endX, double endY)
		{	
			_startX = startX;
			_startY = startY;
			_endX = endX;
			_endY = endY;
			_rect = _getRect(rect, (endY - startY) / (endX - startX));
		}

	int GetX(const double x) const
	{
		return _rect.left + (int)((x - _startX) * GetScale());
	}

	int GetY(const double y) const
	{
		return _rect.bottom - (int)((y - _startY) * GetScale());
	}

	int GetSize(const double size) const
	{
		return (int)(size * GetScale());
	}

	double GetMathSize(int size) const
	{
		return (int)(size / GetScale());
	}

	double GetMathX(int x) {
		return _startX + ((long long int)x - _rect.left) / GetScale();
	}

	double GetMathY(int y) {
		return _startY - ((long long int)y - _rect.bottom) / GetScale();
	}

	RECT _getRect(RECT& rect, double HtoW) {
		RECT r;
		POINT p1, p2;
		if ((long long int)rect.right - rect.left >= ((long long int)rect.bottom - rect.top) / HtoW) {
			p1 = { (rect.right - (LONG)(((long long int)rect.bottom - rect.top) / HtoW + rect.left)) / 2, rect.top };
			p2 = { p1.x + (LONG)(((long long int)rect.bottom - rect.top) / HtoW), rect.bottom };
		}
		else
		{
			p1 = { rect.left, (rect.bottom - (LONG)((rect.right - rect.left) * HtoW + rect.top)) / 2};
			p2 = { rect.right, p1.y + (LONG)((rect.right - rect.left) * HtoW) };
		}
		SetRect(&r, p1.x, p1.y, p2.x, p2.y);
		return r;
	}
private:

	double GetScale() const
	{
		return (double)GetWidth() / (_endX - _startX);
	}
	
	int GetWidth() const
	{
		return _rect.right - _rect.left;
	}
	int GetHeight() const
	{
		return _rect.bottom - _rect.top;
	}
};

class CursorData {
public:
	CursorData() {
		oldPosition = { 0,0 };
		isCliped = false;
		position = {0, 0};
		speedX = 0;
		speedY = 0;
		offset = {0, 0};
	}
	POINT position;
	POINT oldPosition;
	bool isCliped;
	POINT offset;
	int speedX;
	int speedY;
};

class Window
{
	static Room _room;
	static unsigned long _timerPreviousTicks;
	static CursorData _cursorData;
	static Converter _converter;
public:	
	static void InitializeMessageMap()
	{
		MessageMap[WM_CREATE] = OnCreate;
		MessageMap[WM_ERASEBKGND] = OnEraseBackground;
		MessageMap[WM_PAINT] = OnPaint;
		MessageMap[WM_DESTROY] = OnDestroy;
		MessageMap[WM_CHAR] = OnChar;
		MessageMap[WM_MOUSEMOVE] = OnMouseMove;
		MessageMap[WM_LBUTTONDOWN] = OnLButtonDown;
		MessageMap[WM_LBUTTONUP] = OnLButtonUp;
	}

private:
	static LPARAM OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		_timerPreviousTicks = (unsigned long)GetTickCount64();
		SetTimer(nullptr, NULL, USER_TIMER_MINIMUM, TimerProc);
		return true;
	}
	static LPARAM OnEraseBackground(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Ручное стирание кадра.
		return true;
	}
	static LRESULT OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PostQuitMessage(0);
		return 0;
	}
	static LRESULT OnChar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto c = (WCHAR)wParam;
		if (c == ' ')
		{
			if(!_cursorData.isCliped){
				_room.changeBlock();
			}
		}
		return true;
	}
	static LRESULT OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		GetCursorPos(&_cursorData.position);
		ScreenToClient(hWnd, &_cursorData.position);
		if (_cursorData.isCliped) {
			MoveBall(hWnd);
		}
		return 0;
	}
	static LRESULT OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		if (_isMouseOnBall({_converter.GetX(_room.Ball.GetX()), _converter.GetY(_room.Ball.GetY())}, _converter.GetSize(_room.Ball.GetR()))) {
			_clipCursor(hWnd);
		}
		return 0;
	}
	static LRESULT OnLButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		if (_cursorData.isCliped) {
			_unclipCursor();
		}
		return 0;
	}
	static LPARAM OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		_updateConverter(clientRect);

		PAINTSTRUCT ps;
		auto hdc = BeginPaint(hWnd, &ps);
		HDC mhdc = CreateCompatibleDC(hdc);
		HBITMAP bm = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		BitBlt(mhdc, 0, 0, clientRect.right, clientRect.bottom, hdc, 0, 0, SRCCOPY);
		SelectObject(mhdc, bm);

		FillRect(mhdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
		_draw(mhdc);

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mhdc, 0, 0, SRCCOPY);
		DeleteDC(mhdc);
		EndPaint(hWnd, &ps);
		return 0;
	}	
	
	
	static void _draw(HDC hdc) {
		auto ballDrawingCenterX = _converter.GetX(_room.Ball.GetX());
		auto ballDrawingCenterY = _converter.GetY(_room.Ball.GetY());
		auto ballDrawingSize = _converter.GetSize(_room.Ball.GetR());

		HBRUSH ballBrush;
		HPEN ballPen = CreatePen(PS_SOLID, 2, Colors::Black);
		if (_isMouseOnBall({ballDrawingCenterX, ballDrawingCenterY}, ballDrawingSize)) {
			ballBrush = CreateSolidBrush(Colors::Red);
		} else {
			ballBrush = CreateSolidBrush(Colors::Yellow);
		}
		HBRUSH fieldBrush = CreateSolidBrush(Colors::LightGreen);
		HPEN fieldPen = CreatePen(PS_SOLID, 3, Colors::Black);

		RECT Box;
		_setRectFieldsByPoints(&Box, {_converter.GetX(_room.Left), _converter.GetY(_room.Top) }, {_converter.GetX(_room.Right), _converter.GetY(_room.Bottom) });
		_drawRect(hdc, Box, fieldPen, fieldBrush);
		RECT ballRect;
		_setRectFieldsByPoints(&ballRect,{ballDrawingCenterX - ballDrawingSize, ballDrawingCenterY - ballDrawingSize}, {ballDrawingCenterX + ballDrawingSize, ballDrawingCenterY + ballDrawingSize});
		_drawEllipse(hdc, ballRect, ballPen, ballBrush);

		_drawParams(hdc);

		DeleteObject(ballBrush);
		DeleteObject(ballPen);
		DeleteObject(fieldPen);
		DeleteObject(fieldBrush);
	}
	static void _drawParams(HDC hdc) {
		_drawOneParam(hdc, L"X = ", std::to_wstring(_room.Ball.GetX()), 0);
		_drawOneParam(hdc, L"Y = ", std::to_wstring(_room.Ball.GetY()), 1);
		_drawOneParam(hdc, L"Vx = ", std::to_wstring(_room.Ball.GetVX()), 2);
		_drawOneParam(hdc, L"Vy = ", std::to_wstring(_room.Ball.GetVY()), 3);
		_drawOneParam(hdc, L"Ax = ", std::to_wstring(_room.Ball.GetAX()), 4);
		_drawOneParam(hdc, L"Ay = ", std::to_wstring(_room.Ball.GetAY()), 5);
		_drawOneParam(hdc, L"dxLeft = ", std::to_wstring(_room.dxLeft), 6);
		_drawOneParam(hdc, L"dyBottom = ", std::to_wstring(_room.dyBottom), 7);
		_drawOneParam(hdc, L"dxRight = ", std::to_wstring(_room.dxRight), 8);
		_drawOneParam(hdc, L"dyTop = ", std::to_wstring(_room.dyTop), 9);
		_drawOneParam(hdc, L"CursorSpeedX = ", std::to_wstring(_cursorData.speedX), 10);
		_drawOneParam(hdc, L"CursorSpeedY = ", std::to_wstring(_cursorData.speedY), 11);
		_drawOneParam(hdc, L"CursorPostionX = ", std::to_wstring(_cursorData.position.x), 12);
		_drawOneParam(hdc, L"CursorPostionY = ", std::to_wstring(_cursorData.position.y), 13);
	}
	static void _drawOneParam(HDC hdc,std::wstring text1, std::wstring text2, int id) {
		RECT R;
		_setRectFieldsByPoints(&R, { 40, 60 + 30 * id }, { 240, 90 + 30 *id });
		_drawTextInRect(hdc, R, text1 + text2, 15, 1);
	}
	static void _drawTextInRect(HDC hdc, RECT& rect, std::wstring text, int size, int weight)
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
	static void _drawRect(HDC hdc, const RECT& rect, HPEN pen, HBRUSH brush)
	{
		auto oldBrush = SelectObject(hdc, brush);
		auto oldPen = SelectObject(hdc, pen);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
	}
	static void _drawEllipse(HDC hdc, const RECT& rect, HPEN pen, HBRUSH brush) {
		auto oldBrush = SelectObject(hdc, brush);
		auto oldPen = SelectObject(hdc, pen);
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
	}

	static void _cutEdge(RECT* rect, int top, int bottom, int left, int right) {
		SetRect(rect, rect->left + left, rect->top + top, rect->right - right, rect->bottom - bottom);
	}
	static void _updateConverter(RECT rect) {
		_cutEdge(&rect, 10,10,10,10);
		_converter.UpdateRect(rect, _room.Left, _room.Bottom, _room.Right, _room.Top);
	}
	static void _clipCursor(HWND hWnd) {
		_cursorData.isCliped = true;
		_room.Ball.SetSpeed(0, 0);
		_cursorData.offset = { _cursorData.position.x - _converter.GetX(_room.Ball.GetX()), _cursorData.position.y - _converter.GetY(_room.Ball.GetY()) };
		POINT p1 = { _converter.GetX(_room.Left) + _cursorData.offset.x + _converter.GetSize(_room.Ball.GetR()), _converter.GetY(_room.Top) + _cursorData.offset.y + _converter.GetSize(_room.Ball.GetR())};
		POINT p2 = { _converter.GetX(_room.Right) + _cursorData.offset.x - _converter.GetSize(_room.Ball.GetR()), _converter.GetY(_room.Bottom) + _cursorData.offset.y - _converter.GetSize(_room.Ball.GetR())};
		ClientToScreen(hWnd, &p1);
		ClientToScreen(hWnd, &p2);
		RECT clipRect;
		_setRectFieldsByPoints(&clipRect, p1, p2);
		ClipCursor(&clipRect);
		_room.Block();
	}
	static void _unclipCursor() {
		_room.Ball.SetSpeed(_converter.GetMathSize(_cursorData.speedX), -_converter.GetMathSize(_cursorData.speedY));
		ClipCursor(NULL);
		_cursorData.isCliped = false;
		_room.UnBlock();
	}
	static void MoveBall(HWND hWnd) {
		_room.Ball.SetPosition(_converter.GetMathX(_cursorData.position.x - _cursorData.offset.x), _converter.GetMathY(_cursorData.position.y - _cursorData.offset.y));
	}
	static bool _isMouseOnBall(POINT CenterP, int R) {
		int r = (int)sqrt((double)((long long int)CenterP.x - _cursorData.position.x) * ((long long int)CenterP.x - _cursorData.position.x) + ((long long int)CenterP.y - _cursorData.position.y) * ((long long int)CenterP.y - _cursorData.position.y));
		return r < R;
	}
	static void CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR idEvent, DWORD time)
	{
		auto ticksDelta = (time - _timerPreviousTicks) / 1000.0;
		_timerPreviousTicks = time;
		_room.Update(ticksDelta);
		InvalidateRect(hWnd, nullptr, false);
		if (_cursorData.isCliped) {
			_cursorData.speedX = (int)((double)((long long int)_cursorData.position.x - _cursorData.oldPosition.x) / ticksDelta);
			_cursorData.speedY = (int)((double)((long long int)_cursorData.position.y - _cursorData.oldPosition.y) / ticksDelta);
		}
		_cursorData.oldPosition = _cursorData.position;
	}
	static void _setRectFieldsByPoints(RECT* rect, POINT p1, POINT p2) {
		SetRect(rect, p1.x, p1.y, p2.x, p2.y);
	}
};

Room Window::_room;
unsigned long Window::_timerPreviousTicks;
CursorData Window::_cursorData;
Converter Window::_converter;

static int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window::InitializeMessageMap();
	return Application(TEXT("Ball!")).Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}