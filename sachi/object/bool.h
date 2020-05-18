#ifndef SACHI_BOOL_H
#define SACHI_BOOL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_BoolType;

typedef struct _Sachi_Bool Sachi_Bool;

#define Sachi_CheckBool(o) (o->Type == &Sachi_BoolType)

SACHI_PUBLIC(Sachi_Object*) Sachi_NewBool(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(void) Sachi_DeleteBool(Sachi_Object* InObject);	

#ifdef __cplusplus
}
#endif

#endif