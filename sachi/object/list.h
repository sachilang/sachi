#ifndef SACHI_OBJECT_LIST_H
#define SACHI_OBJECT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_ListType;

typedef struct _Sachi_List Sachi_List;

#define Sachi_CheckList(o) (o->Type == &Sachi_ListType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewList(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewListWithCapacity(Sachi_Interpreter* InInterpreter, sachi_size_t InCapacity);
SACHI_PUBLIC(void) Sachi_DeleteList(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) Sachi_IsList(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiList_Empty(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiList_Size(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiList_Reserve(Sachi_Object* InObject, sachi_size_t InSize);
SACHI_PUBLIC(sachi_size_t) SachiList_Capacity(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiList_Push(Sachi_Object* InObject, Sachi_Object* InItem);
SACHI_PUBLIC(Sachi_Object*) SachiList_Pop(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiList_SetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object* InItem);
SACHI_PUBLIC(Sachi_Object*) SachiList_GetItem(Sachi_Object* InObject, sachi_size_t InIndex);
SACHI_PUBLIC(Sachi_Object*) SachiList_Front(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiList_Back(Sachi_Object* InObject);
SACHI_PUBLIC(void) SachiList_Clear(Sachi_Object* InObject);

/**
 * Get the underlying allocated memory array.
 *
 * :param InObject: list
 * :return: underlying memory array
 */
SACHI_PUBLIC(Sachi_Object**) SachiList_Data(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif