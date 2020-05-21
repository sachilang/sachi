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

#define Sachi_IsList(o) (o->Type == &Sachi_ListType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewList(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewListWithCapacity(Sachi_Interpreter* InInterpreter, sachi_size_t InCapacity);
SACHI_PUBLIC(void) Sachi_DeleteList(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiList_Empty(const Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiList_Size(const Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiList_Reserve(Sachi_Object* InObject, sachi_size_t InSize);
SACHI_PUBLIC(sachi_size_t) SachiList_Capacity(const Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiList_Push(Sachi_Object* InObject, Sachi_Object* InItem);
SACHI_PUBLIC(int) SachiList_Pop(Sachi_Object* InObject, Sachi_Object** OutItem);
SACHI_PUBLIC(int) SachiList_SetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object* InItem);
SACHI_PUBLIC(int) SachiList_GetItem(const Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object** OutItem);
SACHI_PUBLIC(Sachi_Object*) SachiList_Front(const Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiList_Back(const Sachi_Object* InObject);
SACHI_PUBLIC(void) SachiList_Clear(Sachi_Object* InObject);
SACHI_PUBLIC(const char*) SachiList_ToString(const Sachi_Object* InObject);

/**
 * Get the underlying allocated memory array.
 *
 * :param InObject: list
 * :return: underlying memory array
 */
SACHI_PUBLIC(Sachi_Object**) SachiList_Data(const Sachi_Object* InObject);

#define SACHILIST_FOREACH(list, index, items) \
    Sachi_Object** items = SachiList_Data(list); \
    sachi_size_t _Size = SachiList_Size(list); \
	for (sachi_size_t index = 0; index < _Size; ++index, items++)

#ifdef __cplusplus
}
#endif

#endif