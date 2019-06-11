#include "Window.h"

#include <string>
#include <tchar.h>
#include <functional>
#include <Windows.h>


Hashira::Window::Window() 
{

}


Hashira::Window::~Window()
{
	Discard();
}


LRESULT Hashira::Window::DefaultWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Hashira::Window::CreateScissorRect(UINT width, UINT height)
{
	// シザー矩形を設定.
	_scissorRect.left = 0;
	_scissorRect.right = width;
	_scissorRect.top = 0;
	_scissorRect.bottom = height;
}

void Hashira::Window::CreateViewPort(UINT width, UINT height)
{
	_viewPort.TopLeftX = 0;
	_viewPort.TopLeftY = 0;
	_viewPort.Width = static_cast<FLOAT>(width);
	_viewPort.Height = static_cast<FLOAT>(height);
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;
}

HRESULT Hashira::Window::Create(std::wstring windowName, UINT width, UINT height, LRESULT(__stdcall *windowProc)(HWND, UINT, WPARAM, LPARAM))
{

	this->_appClassName = windowName;
	_windowProc = this->DefaultWndProc;
	HINSTANCE hInst = GetModuleHandle(nullptr);
	if (!hInst)
	{
		return E_FAIL;
	}
	if (windowProc != nullptr) {
		_windowProc = windowProc;
	}

	WNDCLASSEX wnd = {};
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wnd.lpfnWndProc = _windowProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.lpszClassName = _appClassName.c_str();
	wnd.hInstance = hInst;
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd.hCursor = LoadCursor(NULL, IDC_CROSS);

	RegisterClassEx(&wnd);
	// インスタンスハンドルを設定.
	_hInstance = hInst;

	RECT	windowRect = { 0,0,static_cast<long>(width),static_cast<long>(height) };

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウ作成
	_hWindow = CreateWindow(
		_appClassName.c_str(),
		_appClassName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		_hInstance,
		NULL
	);

	if (!_hWindow) {
		return E_FAIL;
	}
	// ウィンドウを表示
	ShowWindow(_hWindow, SW_SHOWNORMAL);
	UpdateWindow(_hWindow);

	// フォーカス設定.
	SetFocus(_hWindow);

	CreateViewPort(width, height);
	CreateScissorRect(width, height);
	// 正常終了.
	return S_OK;
}

HRESULT Hashira::Window::CustomCreate(std::wstring windowName, UINT width, UINT height, LRESULT(_stdcall*windowProc)(HWND, UINT, WPARAM, LPARAM))
{
	this->_appClassName = windowName;
	this->_windowProc = windowProc;
	HINSTANCE hInst = GetModuleHandle(nullptr);
	if (!hInst)
	{
		return E_FAIL;
	}

	_windowProc = windowProc;

	WNDCLASSEX wnd = {};
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wnd.lpfnWndProc = _windowProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.lpszClassName = _appClassName.c_str();
	wnd.hInstance = hInst;
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wnd);
	// インスタンスハンドルを設定.
	_hInstance = hInst;

	RECT	windowRect = { 0,0,static_cast<long>(width),static_cast<long>(height) };

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウ作成
	_hWindow = CreateWindow(
		_appClassName.c_str(),
		_appClassName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		_hInstance,
		NULL
	);

	if (!_hWindow) {
		return E_FAIL;
	}
	// ウィンドウを表示
	ShowWindow(_hWindow, SW_SHOWNORMAL);
	UpdateWindow(_hWindow);

	// フォーカス設定.
	SetFocus(_hWindow);

	CreateViewPort(width, height);
	CreateScissorRect(width, height);
	// 正常終了.
	return S_OK;
}

HWND & Hashira::Window::GetWindowHandle()
{
	return this->_hWindow;
}

HINSTANCE & Hashira::Window::GetWindowInstance()
{
	return this->_hInstance;
}

D3D12_VIEWPORT  Hashira::Window::GetViewPort()
{
	return _viewPort;
}

D3D12_RECT  Hashira::Window::GetScissorRect()
{
	return _scissorRect;
}

void Hashira::Window::Discard()
{
	if (_hInstance != nullptr)
	{
		UnregisterClassW(_appClassName.c_str(), _hInstance);
	}
	_appClassName.clear();
	_hInstance = nullptr;
	_hWindow = nullptr;
}

