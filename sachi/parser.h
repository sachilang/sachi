#ifndef SACHI_PARSER_H
#define SACHI_PARSER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"

typedef struct _Sachi_Node Sachi_Node;

SACHI_PUBLIC(const char*) SachiParser_GetErrorPtr(void);
SACHI_PUBLIC(int) SachiParser_ReadFile(const char* InFilename, char** OutBuffer, sachi_size_t* OutSize);
SACHI_PUBLIC(Sachi_Node*) SachiParser_Load(const char* InFilename);
SACHI_PUBLIC(Sachi_Node*) SachiParser_Loads(const char* InBuffer);
SACHI_PUBLIC(Sachi_Node*) SachiParser_LoadsWithLength(const char* InBuffer, sachi_size_t InBufferLength);

#ifdef __cplusplus
}
#endif

#endif