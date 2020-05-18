#include "sachi/node/json.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/node.h"

typedef struct _Sachi_JSON
{
	SACHI_OBJECT_HEADER
	Sachi_Object** Items;
	sachi_size_t Size;
	sachi_size_t AllocatedSize;
} Sachi_JSON;

static Sachi_Object* _Sachi_NewJSON(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewJSON(InInterpreter);
}

static void _Sachi_DeleteJSON(Sachi_Object* InObject)
{
	Sachi_DeleteJSON(InObject);
}

static int _SachiJSON_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static Sachi_NodeDef _Sachi_JSONNodes[] = {
	{"init", &_SachiJSON_Init}
};

Sachi_ObjectType Sachi_JSONType = {
	"JSON",
	sizeof(Sachi_JSON),
	NULL, // base
	_Sachi_NewJSON,
	_Sachi_DeleteJSON,
	_Sachi_JSONNodes,
	NULL, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewJSON(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewJSONWithCapacity(InInterpreter, 0);
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewJSONWithCapacity(Sachi_Interpreter* InInterpreter, sachi_size_t InCapacity)
{
	Sachi_JSON* Value = (Sachi_JSON*)Sachi_NewObject(InInterpreter, &Sachi_JSONType);
	if (!Value)
	{
		return NULL;
	}

	Value->Items = NULL;
	Value->Size = 0;
	Value->AllocatedSize = 0;

	SachiJSON_Reserve((Sachi_Object*)Value, InCapacity);

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeleteJSON(Sachi_Object* InObject)
{
	SachiJSON_Clear(InObject);
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(int) SachiJSON_Empty(Sachi_Object* InObject)
{
	return ((Sachi_JSON*)InObject)->Size == 0;
}

SACHI_PUBLIC(sachi_size_t) SachiJSON_Size(Sachi_Object* InObject)
{
	return ((Sachi_JSON*)InObject)->Size;
}

SACHI_PUBLIC(int) SachiJSON_Reserve(Sachi_Object* InObject, sachi_size_t InSize)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	if (JSON->AllocatedSize >= InSize)
	{
		return SACHI_OK;
	}

	Sachi_Object** Items = (Sachi_Object**)sachi_realloc(JSON->Items, sizeof(Sachi_Object*) * InSize);
	if (!Items)
	{
		SachiError_SetMemoryAllocation();
		return SACHI_ERROR;
	}

	JSON->AllocatedSize = InSize;
	JSON->Items = Items;
	
	return SACHI_OK;
}

SACHI_PUBLIC(sachi_size_t) SachiJSON_Capacity(Sachi_Object* InObject)
{
	if (!Sachi_CheckJSON(InObject))
	{
		return SACHI_ERROR;
	}

	return ((Sachi_JSON*)InObject)->AllocatedSize;
}

SACHI_PUBLIC(int) SachiJSON_Push(Sachi_Object* InObject, Sachi_Object* InItem)
{
	if (!Sachi_CheckJSON(InObject))
	{
		return SACHI_ERROR;
	}

	Sachi_JSON* JSON = (Sachi_JSON*)InObject;

	// Must expand allocated memory
	if (JSON->AllocatedSize == JSON->Size)
	{
		if (SachiJSON_Reserve(InObject, JSON->AllocatedSize + 1) != SACHI_OK)
		{
			return SACHI_ERROR;
		}
	}

	Sachi_IncRef(InItem);
	JSON->Items[JSON->Size] = InItem;
	JSON->Size++;

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiJSON_Pop(Sachi_Object* InObject, Sachi_Object** OutItem)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	if (JSON->Size == 0)
	{
		if (OutItem)
		{
			*OutItem = NULL;
		}
		return SACHI_OK;
	}

	JSON->Size--;
	Sachi_Object* Item = JSON->Items[JSON->Size];
	JSON->Items[JSON->Size] = NULL;

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

SACHI_PUBLIC(int) SachiJSON_SetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object* InItem)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	if (InIndex >= JSON->Size)
	{
		return SACHI_ERROR;
	}

	Sachi_IncRef(InItem);
	JSON->Items[InIndex] = InItem;

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiJSON_GetItem(Sachi_Object* InObject, sachi_size_t InIndex, Sachi_Object** OutItem)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	if (InIndex >= JSON->Size)
	{
		return SACHI_ERROR;
	}

	*OutItem = JSON->Items[InIndex];
	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) SachiJSON_Front(Sachi_Object* InObject)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	if (JSON->Size == 0)
	{
		return NULL;
	}

	return JSON->Items[0];
}

SACHI_PUBLIC(Sachi_Object*) SachiJSON_Back(Sachi_Object* InObject)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	if (JSON->Size == 0)
	{
		return NULL;
	}

	return JSON->Items[JSON->Size - 1];
}

SACHI_PUBLIC(void) SachiJSON_Clear(Sachi_Object* InObject)
{
	Sachi_JSON* JSON = (Sachi_JSON*)InObject;
	for (sachi_size_t I = 0; I < JSON->Size; ++I)
	{
		Sachi_Object* Item = JSON->Items[I];
		Item->Type->Delete(Item);
		JSON->Items[I] = NULL;
	}

	JSON->Size = 0;
}

SACHI_PUBLIC(Sachi_Object**) SachiJSON_Data(Sachi_Object* InObject)
{
	return ((Sachi_JSON*)InObject)->Items;
}