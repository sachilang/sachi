#ifndef SACHI_OBJECT_INT_H
#define SACHI_OBJECT_INT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_IntType;

typedef struct _Sachi_Int Sachi_Int;

#define Sachi_CheckInt(o) (o->Type == &Sachi_IntType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewInt(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewIntFromValue(Sachi_Interpreter* InInterpreter, sachi_ssize_t InValue);
SACHI_PUBLIC(void) Sachi_DeleteInt(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_ssize_t) SachiInt_Data(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif