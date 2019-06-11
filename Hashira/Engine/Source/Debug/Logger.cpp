#include <Windows.h>
#include <string>
#include <comdef.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Logger.h"

#define CONVERT_STR(var) #var
const std::string DEBUG_FILE_NAME = "./DebugLog";

namespace Hashira {
	struct ConsoleScreen
	{
		CONSOLE_SCREEN_BUFFER_INFO  ScreenBuffer;

		void BindColor(LOG_LEVEL level);
		void UnBindColor();
	};

	void ConsoleScreen::BindColor(LOG_LEVEL level)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(handle, &ScreenBuffer);

		unsigned short attribute = ScreenBuffer.wAttributes;
		switch (level)
		{
		case LOG_LEVEL::Details:
			attribute = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;

		case LOG_LEVEL::Info:
			attribute = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case LOG_LEVEL::Debug:
			attribute = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case LOG_LEVEL::Warning:
			attribute = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;

		case LOG_LEVEL::Error:
			attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		}

		SetConsoleTextAttribute(handle, attribute);

	}
	void ConsoleScreen::UnBindColor() {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, ScreenBuffer.wAttributes);
	}
}


Hashira::SystemLogger::SystemLogger(): _LogFilter(LOG_LEVEL::Details)
{
#ifdef _DEBUG
	CreateDebugFile();

#endif // _DEBUG
}


Hashira::SystemLogger::~SystemLogger()
{
#ifdef _DEBUG
	UnMap();
#endif // _DEBUG
}


void Hashira::SystemLogger::CreateDebugFile()
{
	time_t localTime = time(nullptr);
	std::string ltt;
	tm lt;
	localtime_s(&lt, &localTime);
	std::stringstream ss;
	ss << DEBUG_FILE_NAME.c_str();
	ss << ".txt";
	std::string res(ss.str());

	_debugFile = std::ofstream(res.c_str());

	Map(res);
}

void Hashira::SystemLogger::Map(std::string fileName)
{
#ifdef _DEBUG
	if (this->_debugFile.is_open()) {
		_debugFile.close();
	}
	this->_debugFile.open(fileName.c_str(), std::ios::out | std::ios::ate);
#endif // _DEBUG
}

void Hashira::SystemLogger::UnMap()
{
#ifdef _DEBUG
	if (this->_debugFile.is_open()) {
		_debugFile.close();
	}
#endif // _DEBUG
}

void Hashira::SystemLogger::Log(LOG_LEVEL level , HRESULT hr)
{
#ifdef _DEBUG
	if (level >= _LogFilter)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		//ConsoleScreen screen;
		//screen.BindColor(level);
		OutputDebugStringW(errMsg);
		Log(level, reinterpret_cast<const char*>(errMsg));
		//screen.UnBindColor();
		//_debugFile << errMsg;
	}
#endif
}

void Hashira::SystemLogger::Log(LOG_LEVEL level, std::string format)
{
#ifdef _DEBUG
	Log(level, format.c_str());
#endif // _DEBUG
}

void Hashira::SystemLogger::Log(LOG_LEVEL level, const char * format, ...)
{
	
#ifdef _DEBUG

	if (level >= _LogFilter) 
	{
		ConsoleScreen screen;
		// ログ出力.
		screen.BindColor(level);
		{
			char msg[2048] = "\0";
			va_list arg;

			va_start(arg, format);
			vsprintf_s(msg, format, arg);
			va_end(arg);

			printf_s("%s", msg);

			OutputDebugStringA(msg);
			_debugFile << msg;
		}
		screen.UnBindColor();
	}
#endif	
}

void Hashira::SystemLogger::LogW(LOG_LEVEL level, std::wstring format)
{
#ifdef _DEBUG

	LogW(level, format.c_str());
#endif
}

void Hashira::SystemLogger::LogW(LOG_LEVEL level, const wchar_t * format, ...)
{
#ifdef _DEBUG

	if (level >= _LogFilter)
	{
		ConsoleScreen screen;
		screen.BindColor(level);
		// ログ出力.
		{
			wchar_t msg[2048] = L"\0";
			va_list arg;

			va_start(arg, format);
			vswprintf_s(msg, format, arg);
			va_end(arg);

			wprintf_s(L"%s", msg);

			OutputDebugStringW(msg);
			_debugFile << msg;
		}
		screen.UnBindColor();
	}
#endif
}

void Hashira::SystemLogger::SetFilter(LOG_LEVEL level)
{
	_LogFilter = level;
}

Hashira::LOG_LEVEL Hashira::SystemLogger::GetFilter()
{
	return _LogFilter;
}

void Hashira::SystemLogger::SetDebugFilePath(std::string filePath)
{
}

Hashira::SystemLogger & Hashira::SystemLogger::GetInstance()
{
	 
	static Hashira::SystemLogger instance;
	return instance;
}
