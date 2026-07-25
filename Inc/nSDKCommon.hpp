#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <vector>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#define _WINDOWS
#define	COXSDK
#define MMFEXT

// TYPEDEFS:
// -----
// 8-bit, unsigned integer
typedef uint8_t uchar, byte, uint8;
// 8-bit, signed integer
typedef int8_t int8;
// 16-bit, signed integer
typedef int16_t int16;
// 16 bit, unsigned integer
typedef uint16_t ushort, word, uint16;
// 32-bit, signed integer
typedef int32_t int32, bool32;
// 32-bit, unsigned integer
typedef unsigned long ulong, dword;
// 32-bit, unsigned integer
typedef uint32_t uint32;
// 64-bit, unsigned integer
typedef uint64_t uint64;
// 64-bit, signed integer
typedef int64_t int64;
// Enough bits to hold a pointer value
typedef uintptr_t uintptr;
// 32-bit, floating-point number
typedef float float32;
// 64-bit, floating-point number
typedef double float64;
// Wide character
typedef wchar_t wchar;
// ASCII/Unicode character (dependong on Unicode define)
#ifdef _UNICODE
typedef wchar tchar;
#else
typedef char tchar;
#endif
// -----

// DEFINES:
// -----
#ifdef _DEBUG
#define EXT_DEBUG
#endif

// When specifying a string in single quotes, the endianness of the characters will not be what we expect, so this reverses the bytes
#define EXT_FIX_IDENTIFIER(id) ((id & 0xFF) << 24) | ((id & 0xFF00) << 8) | ((id & 0xFF0000) >> 8) | ((id & 0xFF000000) >> 24)

#define FUSION_API WINAPI
#define EXT_DLLEXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
// -----

namespace nSDK
{
	extern HINSTANCE hInst;

	namespace Exports
	{
		bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved);
	}
}