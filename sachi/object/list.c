#include "sachi/object/list.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/node.h"

typedef struct _Sachi_List
{
	SACHI_OBJECT_HEADER
	Sachi_Object** Items;
	sachi_size_t Size;
	sachi_size_t AllocatedSize;
} Sachi_List;

static Sachi_Object* _Sachi_NewList(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewList(InInterpreter);
}

static void _Sachi_DeleteList(Sachi_Object* InObject)
{
	Sachi_DeleteList(InObject);
}

static int _SachiList_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static Sachi_NodeDef _Sachi_ListNodes[] = {
	{"init", &_SachiList_Init}
};

Sachi_ObjectType Sachi_ListType = {
	"list",
	_Sachi_NewList,
	_Sachi_DeleteList,
	_Sachi_ListNodes
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewList(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewListWithCapacity(InInterpreter, 0);
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewListWithCapacity(Sachi_Interpreter* InInterpreter, sachi_size_t InCapacity)
{
	Sachi_List* Value = (Sachi_List*)sachi_malloc(sizeof(Sachi_List));
	if (!Value)
	{
		SachiError_SetMemoryAllocation();
		return NULL;
	}

	Value->Type = &Sachi_ListType;
	Value->Interpreter = InInterpreter;
	Value->Items = NULL;
	Value->Size = 0;
	Value->AllocatedSize = 0;

	SachiList_Reserve((Sachi_Object*)Value, InCapacity);

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeleteList(Sachi_Object* InObject)
{

}

SACHI_PUBLIC(Sachi_Object*) Sachi_IsList(Sachi_Object* InObject)
{
	if (InObject->Type == &Sachi_ListType)
	{
		return Sachi_True;
	}

	return Sachi_False;
}

SACHI_PUBLIC(Sachi_Object*) SachiList_Empty(Sachi_Object* InObject)
{
	if (((Sachi_List*)InObject)->Size == 0)
	{
		return Sachi_True;
	}

	return Sachi_False;
}

SACHI_PUBLIC(sachi_size_t) SachiList_Size(Sachi_Object* InObject)
{
	return ((Sachi_List*)InObject)->Size;
}

SACHI_PUBLIC(int) SachiList_Reserve(Sachi_Object* InObject, sachi_size_t InSize)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (List->AllocatedSize >= InSize)
	{
		return SACHI_OK;
	}

	Sachi_Object** Items = (Sachi_Object**)sachi_realloc(List->Items, sizeof(Sachi_Object*) * InSize);
	if (!Items)
	{
		SachiError_SetMemoryAllocation();
		return SACHI_ERROR;
	}

	List->AllocatedSize = InSize;
	List->Items = Items;
	
	return SACHI_OK;
}

SACHI_PUBLIC(sachi_size_t) SachiList_Capacity(Sachi_Object* InObject)
{
	if (!Sachi_CheckList(InObject))
	{
		return SACHI_ERROR;
	}

	return ((Sachi_List*)InObject)->AllocatedSize;
}

SACHI_PUBLIC(int) SachiList_Push(Sachi_Object* InObject, Sachi_Object* InItem)
{
	if (!Sachi_CheckList(InObject))
	{
		return SACHI_ERROR;
	}

	Sachi_List* List = (Sachi_List*)InObject;

	// Must expand allocated memory
	if (List->AllocatedSize == List->Size)
	{
		if (SachiList_Reserve(InObject, List->AllocatedSize + 1) != SACHI_OK)
		{
			return SACHI_ERROR;
		}
	}

	List->Items[List->Size] = InItem;
	List->Size++;

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) SachiList_Pop(Sachi_Object* InObject)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (List->Size == 0)
	{
		return NULL;
	}

	List->Size--;
	Sachi_Object* Item = List->Items[List->Size];
	List->Items[List->Size] = NULL;

	return Item;
}

SACHI_PUBLIC(int) SachiList_SetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object* InItem)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (InIndex >= List->Size)
	{
		return SACHI_ERROR;
	}

	List->Items[InIndex] = InItem;

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) SachiList_GetItem(Sachi_Object* InObject, sachi_size_t InIndex)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (InIndex >= List->Size)
	{
		return NULL;
	}

	return List->Items[InIndex];
}

SACHI_PUBLIC(Sachi_Object*) SachiList_Front(Sachi_Object* InObject)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (List->Size == 0)
	{
		return NULL;
	}

	return List->Items[0];
}

SACHI_PUBLIC(Sachi_Object*) SachiList_Back(Sachi_Object* InObject)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (List->Size == 0)
	{
		return NULL;
	}

	return List->Items[List->Size - 1];
}

SACHI_PUBLIC(void) SachiList_Clear(Sachi_Object* InObject)
{
	Sachi_List* List = (Sachi_List*)InObject;
	for (sachi_size_t I = 0; I < List->Size; ++I)
	{
		Sachi_Object* Item = List->Items[I];
		Item->Type->Delete(Item);
		List->Items[I] = NULL;
	}

	List->Size = 0;
}