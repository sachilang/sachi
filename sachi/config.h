#ifndef SACHI_CONFIG_H
#define SACHI_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#pragma warning(disable:4100)

#define SACHI_LOG_ENABLED 1

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifndef __WINDOWS__
#define __cdecl
#define __stdcall
#endif

#define SACHI_CDECL __cdecl
#define SACHI_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(SACHI_HIDE_SYMBOLS) && !defined(SACHI_IMPORT_SYMBOLS) && !defined(SACHI_EXPORT_SYMBOLS)
#define SACHI_EXPORT_SYMBOLS
#endif

#if defined(SACHI_HIDE_SYMBOLS)
#define SACHI_PUBLIC(type)   type SACHI_STDCALL
#elif defined(SACHI_EXPORT_SYMBOLS)
#define SACHI_PUBLIC(type)   __declspec(dllexport) type SACHI_STDCALL
#elif defined(SACHI_IMPORT_SYMBOLS)
#define SACHI_PUBLIC(type)   __declspec(dllimport) type SACHI_STDCALL
#endif

#include <stddef.h>

#define SACHI_OK 0
#define SACHI_ERROR -1

#ifdef __cplusplus
}
#endif

#endif
