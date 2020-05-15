#include "sachi/object/pin.h"
#include "sachi/interpreter.h"

typedef struct _Sachi_Pin
{
	SACHI_OBJECT_HEADER
	Sachi_PinDef* Defition;
} Sachi_Pin;

static Sachi_Object* _Sachi_NewPin(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewPin(InInterpreter);
}

static void _Sachi_DeletePin(Sachi_Object* InObject)
{
	Sachi_DeletePin(InObject);
}

static int _SachiPin_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static Sachi_PinDef _Sachi_PinNodes[] = {
	{"init", &_SachiPin_Init}
};

Sachi_ObjectType Sachi_PinType = {
	"Pin",
	NULL, // base
	_Sachi_NewPin,
	_Sachi_DeletePin,
	_Sachi_PinNodes,
	NULL, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPin(Sachi_Interpreter* InInterpreter)
{
	Sachi_Pin* Value = (Sachi_Pin*)sachi_malloc(sizeof(Sachi_Pin));
	if (!Value)
	{
		SachiInterpreter_MemoryAllocationError(InInterpreter);
		return NULL;
	}

	Sachi_NewObject(InInterpreter, Value, &Sachi_PinType);
	Value->Defition = NULL;

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeletePin(Sachi_Object* InObject)
{
	Sachi_Pin* Pin = (Sachi_Pin*)InObject;

	Pin->Type = NULL;
	Pin->Defition = NULL;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(Sachi_PinDef*) SachiPin_GetDefition(Sachi_Object* InObject)
{
	return ((Sachi_Pin*)InObject)->Defition;
}

SACHI_PUBLIC(void) SachiPin_SetDefition(Sachi_Object* InObject, Sachi_PinDef* InDefinition)
{
	((Sachi_Pin*)InObject)->Defition = InDefinition;
}

SACHI_PUBLIC(const char*) SachiPin_GetName(Sachi_Object* InObject)
{
	return ((Sachi_Pin*)InObject)->Defition->Name;
}