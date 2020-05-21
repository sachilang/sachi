#ifndef SACHI_OBJECT_JSON_JSONPIN_H
#define SACHI_OBJECT_JSON_JSONPIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPinFromJSON(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict);

SACHI_PUBLIC(int) SachiPin_InitFromJSON(Sachi_Object* InObject, Sachi_Object* InDict);

#ifdef __cplusplus
}
#endif

#endif