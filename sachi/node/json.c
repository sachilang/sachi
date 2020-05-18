#include "sachi/node/json.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/pin.h"
#include "sachi/object/node.h"
#include "sachi/object/dict.h"
#include "sachi/object/string.h"

#include "cJSON/cJSON.h"

static Sachi_PinDef _LoadsDefs[] = {
	{NULL, "execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN, NULL, NULL},
	{NULL, "execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT, NULL, NULL},
	{NULL, "s", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN, NULL, NULL},
	NULL
};

static int _SachiJSON_Loads(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults)
{
	Sachi_Object* Item;
	if (SachiDict_GetItemFromBuffer(InKwArgs, "s", &Item) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	cJSON* Root = cJSON_ParseWithLength(SachiString_Data(Item), SachiString_Size(Item));

	return SACHI_OK;
}

static Sachi_NodeDef _MethodsDefs[] = {
	{"loads", _SachiJSON_Loads, _LoadsDefs},
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