#ifndef SACHI_H
#define SACHI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"

SACHI_PUBLIC(const char*) Sachi_GetErrorPtr(void);
SACHI_PUBLIC(void) Sachi_SetErrorPtr(const char* InBuffer);
SACHI_PUBLIC(void) SachiError_SetMemoryAllocation();

SACHI_PUBLIC(LONG) Sachi_HashFromBuffer(const char* InBuffer);
SACHI_PUBLIC(LONG) Sachi_HashFromBufferAndLength(const char* InBuffer, sachi_size_t InLength);

#ifdef WITH_MEMORY_TRACKER
typedef struct _Sachi_Object Sachi_Object;
typedef struct _Sachi_MemoryTracker Sachi_MemoryTracker;
typedef struct _Sachi_MemoryTracker
{
	Sachi_MemoryTracker* Next;
	Sachi_Object* Object;
} Sachi_MemoryTracker;

extern Sachi_MemoryTracker MemoryTracker;

SACHI_PUBLIC(void) Sachi_TrackObject(Sachi_Object* InObject);
SACHI_PUBLIC(void) Sachi_UntrackObject(Sachi_Object* InObject);
#endif


SACHI_PUBLIC(int) Sachi_ReadFile(const char* InFilename, char** OutBuffer, sachi_size_t* OutSize);
SACHI_PUBLIC(const char*) Sachi_Join(const char* InLeft, const char* InRight);

#ifdef __cplusplus
}
#endif

#endif
