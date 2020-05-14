#ifndef SACHI_FUNC_H
#define SACHI_FUNC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_FuncType;

typedef struct _Sachi_Func
{
	Sachi_Object Base;
	char* Name; // Unique name to reference this function
} Sachi_Func;

#ifdef __cplusplus
}
#endif

#endif