#ifndef SACHI_OBJECT_DICT_H
#define SACHI_OBJECT_DICT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_DictType;

typedef struct _Sachi_Dict Sachi_Dict;

#define Sachi_CheckDict(o) (o->Type == &Sachi_DictType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewDict(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(void) Sachi_DeleteDict(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiDict_Empty(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiDict_Size(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiDict_SetItem(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object* InValue);
SACHI_PUBLIC(Sachi_Object*) SachiDict_GetItem(Sachi_Object* InObject, Sachi_Object* InKey);
SACHI_PUBLIC(Sachi_Object*) SachiDict_RemoveItem(Sachi_Object* InObject, Sachi_Object* InKey);

#ifdef __cplusplus
}
#endif

#endif