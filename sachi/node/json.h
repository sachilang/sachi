#ifndef SACHI_NODE_JSON_H
#define SACHI_NODE_JSON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_JSONType;

typedef struct _Sachi_JSON Sachi_JSON;

#define Sachi_CheckJSON(o) (o->Type == &Sachi_JSONType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewJSON(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewJSONWithCapacity(Sachi_Interpreter* InInterpreter, sachi_size_t InCapacity);
SACHI_PUBLIC(void) Sachi_DeleteJSON(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiJSON_Empty(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiJSON_Size(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiJSON_Reserve(Sachi_Object* InObject, sachi_size_t InSize);
SACHI_PUBLIC(sachi_size_t) SachiJSON_Capacity(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiJSON_Push(Sachi_Object* InObject, Sachi_Object* InItem);
SACHI_PUBLIC(int) SachiJSON_Pop(Sachi_Object* InObject, Sachi_Object** OutItem);
SACHI_PUBLIC(int) SachiJSON_SetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object* InItem);
SACHI_PUBLIC(int) SachiJSON_GetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object** OutItem);
SACHI_PUBLIC(Sachi_Object*) SachiJSON_Front(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiJSON_Back(Sachi_Object* InObject);
SACHI_PUBLIC(void) SachiJSON_Clear(Sachi_Object* InObject);

/**
 * Get the underlying allocated memory array.
 *
 * :param InObject: JSON
 * :return: underlying memory array
 */
SACHI_PUBLIC(Sachi_Object**) SachiJSON_Data(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif