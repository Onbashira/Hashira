#pragma once

#include <cstdint>
#include <string>
#include <functional>

namespace Hashira
{
	typedef float       Float32; ///< 32-bit float

	typedef int64_t     Int64;   ///< 64-bit signed integer
	typedef int32_t     Int32;   ///< 32-bit signed integer
	typedef int16_t     Int16;   ///< 16-bit signed integer
	typedef int8_t      Int8;    ///< 8-bit signed integer

	typedef uint64_t    Uint64;  ///< 64-bit unsigned integer
	typedef uint32_t    Uint32;  ///< 32-bit unsigned integer
	typedef uint16_t    Uint16;  ///< 16-bit unsigned integer
	typedef uint8_t     Uint8;   ///< 8-bit unsigned integer

	typedef size_t SizeType;
	typedef void* PVoid;
	typedef std::function<void(void)> Action;

	typedef bool Bool;          ///< Boolean
	static constexpr Bool False = false;
	static constexpr Bool True = true;

	typedef std::basic_string<char> String; ///< String variable

	typedef LRESULT(__stdcall *WindowProcFunc)(HWND, UINT, WPARAM, LPARAM);
}