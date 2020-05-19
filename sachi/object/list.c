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

static const char* _SachiList_ToString(Sachi_Object* InObject)
{
	return SachiList_ToString(InObject);
}

static Sachi_NodeDef _Sachi_ListNodes[] = {
	{"init", &_SachiList_Init}
};

Sachi_ObjectType Sachi_ListType = {
	"list",
	sizeof(Sachi_List),
	NULL, // base
	_Sachi_NewList,
	_Sachi_DeleteList,
	_Sachi_ListNodes,
	NULL, // hash,
	_SachiList_ToString
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewList(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewListWithCapacity(InInterpreter, 0);
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewListWithCapacity(Sachi_Interpreter* InInterpreter, sachi_size_t InCapacity)
{
	Sachi_List* Value = (Sachi_List*)Sachi_NewObject(InInterpreter, &Sachi_ListType);
	if (!Value)
	{
		return NULL;
	}

	Value->Items = NULL;
	Value->Size = 0;
	Value->AllocatedSize = 0;

	SachiList_Reserve((Sachi_Object*)Value, InCapacity);

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeleteList(Sachi_Object* InObject)
{
	SachiList_Clear(InObject);
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(int) SachiList_Empty(Sachi_Object* InObject)
{
	return ((Sachi_List*)InObject)->Size == 0;
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
	Sachi_List* List = (Sachi_List*)InObject;

	// Must expand allocated memory
	if (List->AllocatedSize == List->Size)
	{
		if (SachiList_Reserve(InObject, List->AllocatedSize + 1) != SACHI_OK)
		{
			return SACHI_ERROR;
		}
	}

	Sachi_IncRef(InItem);
	List->Items[List->Size] = InItem;
	List->Size++;

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiList_Pop(Sachi_Object* InObject, Sachi_Object** OutItem)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (List->Size == 0)
	{
		if (OutItem)
		{
			*OutItem = NULL;
		}
		return SACHI_OK;
	}

	List->Size--;
	Sachi_Object* Item = List->Items[List->Size];
	List->Items[List->Size] = NULL;

	if (OutItem)
	{
		*OutItem = Item;
	}

	if (Sachi_DecRef(Item) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiList_SetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object* InItem)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (InIndex >= List->Size)
	{
		return SACHI_ERROR;
	}

	Sachi_Object* OldItem = List->Items[InIndex];
	List->Items[InIndex] = InItem;
	Sachi_DecRef(OldItem);
	Sachi_IncRef(InItem);

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiList_GetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object** OutItem)
{
	Sachi_List* List = (Sachi_List*)InObject;
	if (InIndex >= List->Size)
	{
		return SACHI_ERROR;
	}

	*OutItem = List->Items[InIndex];
	return SACHI_OK;
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
		List->Items[I] = NULL;
		Sachi_DecRef(Item);
	}

	List->Size = 0;
}

SACHI_PUBLIC(const char*) SachiList_ToString(Sachi_Object* InObject)
{
	Sachi_List* List = (Sachi_List*)InObject;

	char* Buffer = sachi_malloc(sizeof(char) * 2);
	Buffer[0] = '[';
	Buffer[1] = '\0';
	const char* Tmp = Buffer;
	Sachi_Object** Entry = List->Items;
	for (sachi_size_t I = List->Size; I > 0; --I)
	{
		const char* Result = Sachi_Join(Tmp, (*Entry)->Type->ToString(*Entry));
		sachi_free(Tmp);
		Tmp = Result;
		if (I > 1)
		{
			Result = Sachi_Join(Tmp, ", ");
			sachi_free(Tmp);
			Tmp = Result;
		}
		Entry++;
	}
	const char* Result = Sachi_Join(Tmp, "]");
	sachi_free(Tmp);
	return Result;
}

SACHI_PUBLIC(Sachi_Object**) SachiList_Data(Sachi_Object* InObject)
{
	return ((Sachi_List*)InObject)->Items;
}