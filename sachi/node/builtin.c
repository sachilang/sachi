#include "sachi/node/json.h"
#include "sachi/sachi.h"
#include "sachi/object/bool.h"
#include "sachi/object/int.h"
#include "sachi/object/string.h"
#include "sachi/object/pin.h"
#include "sachi/object/node.h"
#include "sachi/object/nodeinstance.h"
#include "sachi/object/dict.h"
#include "sachi/object/list.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"

#include "cJSON/cJSON.h"

static Sachi_PinMetadata _IfPins[] = {
	{"exec", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"true", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"false", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"condition", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	NULL
};

static int _If(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	return SACHI_OK;
}

Sachi_NodeMetadata Sachi_IfNodeMetadata = {
	"if",
	_If,
	_IfPins,
	NULL, // nodes
};

static Sachi_PinMetadata _ForInPins[] = {
	{"exec", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"loop", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"done", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"iterable", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	{"item", SACHI_PINMODE_VALUE, SACHI_PINSIDE_OUT},
	NULL
};

static int _ForIn(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	return SACHI_OK;
}

Sachi_NodeMetadata Sachi_ForInNodeMetadata = {
	"forin",
	_ForIn,
	_ForInPins,
	NULL, // nodes
};

static Sachi_PinMetadata _WhilePins[] = {
	{"exec", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"loop", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"done", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"condition", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	NULL
};

static int _While(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	// Retrieve last state
	Sachi_Object* Condition = NULL;
	if (SachiNodeInstance_GetLocalFromBuffer(InNodeInstance, "condition", Condition) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	// Initialize state
	if (!Condition)
	{
	}

	return SACHI_OK;
}

Sachi_NodeMetadata Sachi_WhileNodeMetadata = {
	"while",
	_While,
	_WhilePins,
	NULL, // nodes
};

static Sachi_PinMetadata _ImportPins[] = {
	{"execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"node", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	{"name", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	NULL
};

static int _Import(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	return SACHI_OK;
}

Sachi_NodeMetadata Sachi_ImportNodeMetadata = {
	"import",
	_Import,
	_ImportPins,
	NULL, // nodes
};

static Sachi_PinMetadata _PrintPins[] = {
	{"execin", SACHI_PINMODE_EXEC, SACHI_PINSIDE_IN},
	{"execout", SACHI_PINMODE_EXEC, SACHI_PINSIDE_OUT},
	{"o", SACHI_PINMODE_VALUE, SACHI_PINSIDE_IN},
	NULL
};

static int _Print(Sachi_Object* InNodeInstance, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* InKwResults)
{
	return SACHI_OK;
}

Sachi_NodeMetadata Sachi_PrintNodeMetadata = {
	"print",
	_Print,
	_PrintPins,
	NULL, // nodes
};

Sachi_NodeMetadata* Sachi_BuiltinNodesMetadata[] = {
	&Sachi_ImportNodeMetadata,
	&Sachi_PrintNodeMetadata,
	NULL,
};