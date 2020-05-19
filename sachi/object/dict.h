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
SACHI_PUBLIC(int) SachiDict_Empty(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiDict_Size(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiDict_SetItem(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object* InValue);
SACHI_PUBLIC(int) SachiDict_SetItemFromBuffer(Sachi_Object* InObject, const char* InBuffer, Sachi_Object* InValue);
SACHI_PUBLIC(int) SachiDict_SetItemFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength, Sachi_Object* InValue);
SACHI_PUBLIC(int) SachiDict_GetItem(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object** OutItem);
SACHI_PUBLIC(int) SachiDict_GetItemFromBuffer(Sachi_Object* InObject, const char* InBuffer, Sachi_Object** OutItem);
SACHI_PUBLIC(int) SachiDict_GetItemFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutItem);
SACHI_PUBLIC(int) SachiDict_GetItemFromHash(Sachi_Object* InObject, LONG InHash, Sachi_Object** OutItem);
SACHI_PUBLIC(int) SachiDict_RemoveItem(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object** OutItem);
SACHI_PUBLIC(void) SachiDict_Clear(Sachi_Object* InObject);
SACHI_PUBLIC(const char*) SachiDict_ToString(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif