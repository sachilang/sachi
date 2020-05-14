#include "sachi/object/node.h"
#include "sachi/object/pin.h"
#include "sachi/object/list.h"
#include "sachi/interpreter.h"

typedef struct _Sachi_Node
{
    SACHI_OBJECT_HEADER
    Sachi_NodeDef* Defition;
	Sachi_Object* Pins;
} Sachi_Node;

static Sachi_Object* _Sachi_NewNode(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewNode(InInterpreter);
}

static void _Sachi_DeleteNode(Sachi_Object* InObject)
{
	Sachi_DeleteNode(InObject);
}

static int _SachiNode_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static Sachi_NodeDef _Sachi_NodeNodes[] = {
	{"init", &_SachiNode_Init}
};

Sachi_ObjectType Sachi_NodeType = {
	"node",
	_Sachi_NewNode,
	_Sachi_DeleteNode,
	_Sachi_NodeNodes
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNode(Sachi_Interpreter* InInterpreter)
{
	Sachi_Node* Value = (Sachi_Node*)sachi_malloc(sizeof(Sachi_Node));
	if (!Value)
	{
		SachiInterpreter_MemoryAllocationError(InInterpreter);
		return NULL;
	}

	Value->Type = &Sachi_NodeType;
	Value->Interpreter = InInterpreter;
	Value->Defition = NULL;
	Value->Pins = Sachi_NewList(InInterpreter);
	if (!Value->Pins)
	{
		Sachi_DeleteNode(Value);
		return NULL;
	}

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeleteNode(Sachi_Object* InObject)
{
	if (!InObject)
	{
		return;
	}

	Sachi_Node* Node = (Sachi_Node*)InObject;

	Node->Type = NULL;
	Node->Defition = NULL;
	Sachi_DeleteList(Node->Pins);
	Node->Pins = NULL;
	sachi_free(InObject);
}

SACHI_PUBLIC(Sachi_NodeDef*) SachiNode_GetDefition(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Defition;
}

SACHI_PUBLIC(int) SachiNode_SetDefition(Sachi_Object* InObject, Sachi_NodeDef* InDefinition)
{
	Sachi_Node* Node = (Sachi_Node*)InObject;

	Node->Defition = InDefinition;
	SachiList_Clear(Node->Pins);

	Sachi_PinDef* PinDef = InDefinition->Pins;
	while (PinDef && PinDef->Name != NULL)
	{
		Sachi_Pin* Pin = Sachi_NewPin(Node->Interpreter);
		if (!Pin)
		{
			return SACHI_ERROR;
		}
		SachiPin_SetDefition(Pin, PinDef);
		SachiList_Push(Node->Pins, Pin);
		PinDef++;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPins(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Pins;
}