#include "sachi/object/dict.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/node.h"
#include "sachi/interpreter.h"

typedef struct _Sachi_DictEntry _Sachi_DictEntry;
typedef struct _Sachi_DictEntry
{
	SACHI_OBJECT_HEADER
	_Sachi_DictEntry* Next;
	Sachi_Object* Key;
	Sachi_Object* Value;
	LONG Hash;
} _Sachi_DictEntry;

static _Sachi_DictEntry* _Sachi_NewDictEntry(Sachi_Interpreter* InInterpreter)
{
	_Sachi_DictEntry* Value = (_Sachi_DictEntry*)sachi_malloc(sizeof(_Sachi_DictEntry));
	if (!Value)
	{
		SachiError_SetMemoryAllocation();
		return NULL;
	}

	Value->Type = &Sachi_DictType;
	Value->Interpreter = InInterpreter;
	Value->Next = NULL;
	Value->Key = NULL;
	Value->Value = NULL;
	Value->Hash = 0;

	return (Sachi_Object*)Value;
}

static void _Sachi_DeleteDictEntry(Sachi_Object* InObject)
{
	_Sachi_DictEntry* Value = (_Sachi_DictEntry*)InObject;
	sachi_free(Value);
}

typedef struct _Sachi_Dict
{
	SACHI_OBJECT_HEADER
	_Sachi_DictEntry* Entry;
	sachi_size_t Size;
} Sachi_Dict;

static Sachi_Object* _Sachi_NewDict(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewDict(InInterpreter);
}

static void _Sachi_DeleteDict(Sachi_Object* InObject)
{
	Sachi_DeleteDict(InObject);
}

static int _SachiDict_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static Sachi_NodeDef _Sachi_DictNodes[] = {
	{"init", &_SachiDict_Init}
};

Sachi_ObjectType Sachi_DictType = {
	"dict",
	NULL, // base
	_Sachi_NewDict,
	_Sachi_DeleteDict,
	_Sachi_DictNodes,
	NULL, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewDict(Sachi_Interpreter* InInterpreter)
{
	Sachi_Dict* Value = (Sachi_Dict*)sachi_malloc(sizeof(Sachi_Dict));
	if (!Value)
	{
		SachiError_SetMemoryAllocation();
		return NULL;
	}

	Value->Type = &Sachi_DictType;
	Value->Interpreter = InInterpreter;
	Value->Entry = NULL;
	Value->Size = 0;

	Value->Entry = _Sachi_NewDictEntry(InInterpreter);
	if (!Value->Entry)
	{
		Sachi_DeleteDict(Value);
		return NULL;
	}

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeleteDict(Sachi_Object* InObject)
{
	SachiDict_Clear(InObject);
	sachi_free(InObject);
}

SACHI_PUBLIC(Sachi_Object*) SachiDict_Empty(Sachi_Object* InObject)
{
	if (((Sachi_Dict*)InObject)->Size == 0)
	{
		return Sachi_True;
	}

	return Sachi_False;
}

SACHI_PUBLIC(sachi_size_t) SachiDict_Size(Sachi_Object* InObject)
{
	return ((Sachi_Dict*)InObject)->Size;
}

static Sachi_Object* _SachiDict_FindEntry(Sachi_Object* InObject, LONG InHash, _Sachi_DictEntry** OutPrevious)
{
	Sachi_Dict* Dict = (Sachi_Dict*)InObject;
	_Sachi_DictEntry* Previous = NULL;
	_Sachi_DictEntry* Entry = Dict->Entry;
	while (Entry->Next)
	{
		Previous = Entry;
		Entry = Entry->Next;

		if (Entry->Hash == InHash)
		{
			if (OutPrevious)
			{
				*OutPrevious = Previous;
			}
			return Entry;
		}
	}

	return NULL;
}

SACHI_PUBLIC(int) SachiDict_SetItem(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object* InValue)
{
	LONG Hash = InKey->Type->Hash(InKey);
	if (Hash == -1)
	{
		return NULL;
	}

	_Sachi_DictEntry* Entry = (_Sachi_DictEntry*)_SachiDict_FindEntry(InObject, Hash, NULL);
	if (!Entry)
	{
		Entry = _Sachi_NewDictEntry(InObject->Interpreter);
		if (!Entry)
		{
			SachiInterpreter_MemoryAllocationError(InObject->Interpreter);
			return SACHI_ERROR;
		}

		Sachi_Dict* Dict = (Sachi_Dict*)InObject;
		Entry->Next = Dict->Entry->Next;
		Entry->Hash = Hash;
		Entry->Key = InKey;
		Dict->Entry->Next = Entry;
		Dict->Size++;
	}

	Entry->Value = InValue;

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) SachiDict_GetItem(Sachi_Object* InObject, Sachi_Object* InKey)
{
	LONG Hash = InKey->Type->Hash(InKey);
	if (Hash == -1)
	{
		return NULL;
	}

	_Sachi_DictEntry* Entry = (_Sachi_DictEntry*)_SachiDict_FindEntry(InObject, Hash, NULL);
	if (!Entry)
	{
		return NULL;
	}

	return Entry->Value;
}

SACHI_PUBLIC(Sachi_Object*) SachiDict_RemoveItem(Sachi_Object* InObject, Sachi_Object* InKey)
{
	LONG Hash = InKey->Type->Hash(InKey);
	if (Hash == -1)
	{
		return NULL;
	}

	_Sachi_DictEntry* Previous = NULL;
	_Sachi_DictEntry* Entry = (_Sachi_DictEntry*)_SachiDict_FindEntry(InObject, Hash, &Previous);
	if (!Entry)
	{
		return NULL;
	}

	Previous->Next = Entry->Next;
	((Sachi_Dict*)InObject)->Size--;
	Sachi_Object* Value = Entry->Value;
	_Sachi_DeleteDictEntry(Entry);

	return Value;
}

SACHI_PUBLIC(void) SachiDict_Clear(Sachi_Object* InObject)
{
	Sachi_Dict* Dict = (Sachi_Dict*)InObject;

	_Sachi_DictEntry* Entry = Dict->Entry;
	_Sachi_DictEntry* Next = NULL;
	while (Entry->Next)
	{
		Next = Entry->Next;
		Entry->Next = Next->Next;
		_Sachi_DeleteDictEntry(Next);
	}

	Dict->Size = 0;
}