#include "sachi/node/json.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/pin.h"
#include "sachi/object/node.h"
#include "sachi/object/dict.h"
#include "sachi/object/list.h"
#include "sachi/object/string.h"

#include "cJSON/cJSON.h"

static Sachi_PinDef _LoadsDefs[] = {
	{NULL, "execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT, NULL, NULL},
	{NULL, "s", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "o", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT, NULL, NULL},
	NULL
};

static Sachi_PinDef _LoadDefs[] = {
	{NULL, "execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT, NULL, NULL},
	{NULL, "s", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "o", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT, NULL, NULL},
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
		Object = Sachi_NewList(InInterpreter, cJSON_GetArraySize(InObject));
	}
	else if (cJSON_IsString(InObject))
	{
		Object = Sachi_NewStringFromBuffer(InInterpreter, InObject->valuestring);
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
			Sachi_Object* Value = Sachi_NewStringFromBuffer(InInterpreter, Child->valuestring);
			SachiDict_SetItemFromBuffer(Object, Child->string, Value);
			Sachi_DecRef(Value);
		}
	}
	else if (cJSON_IsArray(InObject))
	{
		Object = Sachi_NewList(InInterpreter, cJSON_GetArraySize(InObject));
	}

	*OutObject = Object;
	return SACHI_OK;
}

static int _Loads(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutObject)
{
	cJSON* Root = cJSON_ParseWithLength(InBuffer, InLength);
	if (!Root)
	{
		return SACHI_ERROR;
	}

	return _Convert(InInterpreter, Root, OutObject);
}

static int _SachiJSON_Loads(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults)
{
	Sachi_Object* Item;
	if (SachiDict_GetItemFromBuffer(InKwArgs, "s", &Item) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_Object* Result = NULL;
	if (_Loads(InInterpreter, SachiString_Data(Item), SachiString_Size(Item), &Result) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_SetItemFromBuffer(OutKwResults, "o", Result) != SACHI_OK)
	{
		Sachi_DecRef(Result);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Result);
	return SACHI_OK;
}

static int _SachiJSON_Load(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults)
{
	Sachi_Object* Item;
	if (SachiDict_GetItemFromBuffer(InKwArgs, "s", &Item) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	char* Buffer = NULL;
	sachi_size_t Size = 0;
	if (Sachi_ReadFile(SachiString_Data(Item), Buffer, Size) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_Object* Result = NULL;
	if (_Loads(InInterpreter, Buffer, Size, &Result) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_SetItemFromBuffer(OutKwResults, "o", Result) != SACHI_OK)
	{
		Sachi_DecRef(Result);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Result);
	return SACHI_OK;
}

static Sachi_NodeDef _MethodsDefs[] = {
	{"loads", _SachiJSON_Loads, _LoadsDefs},
	{"load", _SachiJSON_Load, _LoadDefs},
	NULL
};

static Sachi_NodeDef _NodeDef = {
	"package",
	NULL,
	NULL,
	_MethodsDefs
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewJSON(Sachi_Interpreter* InInterpreter)
{
	Sachi_Object* Node = Sachi_NewNode(InInterpreter);
	if (!Node)
	{
		return NULL;
	}

	if (SachiNode_SetDefition(Node, &_NodeDef) != SACHI_OK)
	{
		Sachi_DeleteNode(Node);
		return NULL;
	}

	return Node;
}

SACHI_PUBLIC(void) Sachi_DeleteJSON(Sachi_Object* InObject)
{
	Sachi_DeleteObject(InObject);
}