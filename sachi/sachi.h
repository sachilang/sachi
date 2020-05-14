#ifndef SACHI_H
#define SACHI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"

SACHI_PUBLIC(const char*) Sachi_GetErrorPtr(void);
SACHI_PUBLIC(void) Sachi_SetErrorPtr(const char* InBuffer);
SACHI_PUBLIC(void) SachiError_SetMemoryAllocation();

#ifdef __cplusplus
}
#endif

#endif
