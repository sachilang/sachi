#include "sachi/node/json.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/int.h"
#include "sachi/object/string.h"
#include "sachi/object/pin.h"
#include "sachi/object/node.h"
#include "sachi/object/dict.h"
#include "sachi/object/list.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"

#include "cJSON/cJSON.h"

static Sachi_PinMetadata _LoadsPins[] = {
	{"execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"s", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	{"o", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT},
	NULL
};

static Sachi_PinMetadata _LoadPins[] = {
	{"execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"s", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	{"o", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT},
	NULL
};

static int _Convert(Sachi_Interpreter* InInterpreter, cJSON* InObject, Sachi_Object** OutObject)
{
	Sachi_Object* Object = NULL;
	if (cJSON_IsObject(InObject))
	{
		Object = Sachi_NewDict(InInterpreter);
	}
	else if (cJSON_IsArray(InObject))
	{
		Object = Sachi_NewListWithCapacity(InInterpreter, cJSON_GetArraySize(InObject));
	}
	else if (cJSON_IsString(InObject))
	{
		Object = Sachi_NewStringFromBuffer(InInterpreter, InObject->valuestring);
	}
	else if (cJSON_IsNumber(InObject))
	{
		Object = Sachi_NewIntFromValue(InInterpreter, InObject->valueint);
	}
	else if (cJSON_IsBool(InObject))
	{
		if (cJSON_IsTrue(InObject))
		{
			Object = Sachi_True((Sachi_Object*)InInterpreter);
		}
		else
		{
			Object = Sachi_False((Sachi_Object*)InInterpreter);
		}
	}

	if (!Object)
	{
		return SACHI_ERROR;
	}

	if (cJSON_IsObject(InObject))
	{
		int Size = cJSON_GetArraySize(InObject);
		for (int I = 0; I < Size; ++I)
		{
			cJSON* Child = cJSON_GetArrayItem(InObject, I);
			Sachi_Object* Value = NULL;
			if (_Convert(InInterpreter, Child, &Value) != SACHI_OK)
			{
				Sachi_DecRef(Object);
				return SACHI_ERROR;
			}
			if (SachiDict_SetItemFromBuffer(Object, Child->string, Value) != SACHI_OK)
			{
				Sachi_DecRef(Value);
				Sachi_DecRef(Object);
				return SACHI_ERROR;
			}
			Sachi_DecRef(Value);
		}
	}
	else if (cJSON_IsArray(InObject))
	{
		int Size = cJSON_GetArraySize(InObject);
		for (int I = 0; I < Size; ++I)
		{
			cJSON* Child = cJSON_GetArrayItem(InObject, I);
			Sachi_Object* Value = NULL;
			if (_Convert(InInterpreter, Child, &Value) != SACHI_OK)
			{
				Sachi_DecRef(Object);
				return SACHI_ERROR;
			}
			if (SachiList_Push(Object, Value) != SACHI_OK)
			{
				Sachi_DecRef(Value);
				Sachi_DecRef(Object);
				return SACHI_ERROR;
			}
			Sachi_DecRef(Value);
		}
	}

	*OutObject = Object;
	return SACHI_OK;
}

static int _SachiJSON_Loads(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	Sachi_Object* Item;
	if (SachiDict_GetItemFromBuffer(InKwArgs, "s", &Item) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_Object* Result = NULL;
	if (SachiJSON_Loads(InNodeInstance->Interpreter, Item, &Result) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_SetItemFromBuffer(InKwResults, "o", Result) != SACHI_OK)
	{
		Sachi_DecRef(Result);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Result);
	return SACHI_OK;
}

static int _SachiJSON_Load(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	Sachi_Object* Item;
	if (SachiDict_GetItemFromBuffer(InKwArgs, "s", &Item) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	char* Buffer = NULL;
	sachi_size_t Size = 0;
	if (Sachi_ReadFile(SachiString_Data(Item), &Buffer, &Size) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_Object* Result = NULL;
	if (SachiJSON_LoadsFromBufferAndLength(InNodeInstance->Interpreter, Buffer, Size, &Result) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_SetItemFromBuffer(InKwResults, "o", Result) != SACHI_OK)
	{
		Sachi_DecRef(Result);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Result);
	return SACHI_OK;
}

static Sachi_NodeMetadata _Methods[] = {
	{"loads", _SachiJSON_Loads, _LoadsPins},
	{"load", _SachiJSON_Load, _LoadPins},
	NULL
};

Sachi_NodeMetadata Sachi_JSONNodeMetadata = {
	"package",
	NULL, // func
	NULL, // pins
	_Methods
};

SACHI_PUBLIC(int) SachiJSON_Loads(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object** OutObject)
{
	return SachiJSON_LoadsFromBufferAndLength(InInterpreter, SachiString_Data(InObject), SachiString_Size(InObject), OutObject);
}

SACHI_PUBLIC(int) SachiJSON_LoadsFromBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer, Sachi_Object** OutObject)
{
	return SachiJSON_LoadsFromBufferAndLength(InInterpreter, InBuffer, sachi_strlen(InBuffer), OutObject);
}

SACHI_PUBLIC(int) SachiJSON_LoadsFromBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutObject)
{
	cJSON* Root = cJSON_ParseWithLength(InBuffer, InLength);
	if (!Root)
	{
		return SACHI_ERROR;
	}

	return _Convert(InInterpreter, Root, OutObject);
}