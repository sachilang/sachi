#include "sachi/object/node.h"
#include "sachi/object/pin.h"
#include "sachi/object/list.h"
#include "sachi/object/interpreter.h"

typedef struct _Sachi_Node
{
    SACHI_OBJECT_HEADER
    Sachi_NodeDef* Defition;
	Sachi_Object* Pins;
	Sachi_Object* Children;
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
	sizeof(Sachi_Node),
	NULL, // base
	_Sachi_NewNode,
	_Sachi_DeleteNode,
	_Sachi_NodeNodes,
	NULL, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNode(Sachi_Interpreter* InInterpreter)
{
	Sachi_Node* Value = (Sachi_Node*)Sachi_NewObject(InInterpreter, &Sachi_NodeType);
	if (!Value)
	{
		return NULL;
	}

	Value->Defition = NULL;
	Value->Pins = NULL;
	Value->Children = NULL;

	Value->Pins = Sachi_NewList(InInterpreter);
	if (!Value->Pins)
	{
		Sachi_DeleteNode(Value);
		return NULL;
	}

	Value->Children = Sachi_NewList(InInterpreter);
	if (!Value->Children)
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
	Sachi_DeleteList(Node->Children);
	Node->Children = NULL;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(Sachi_NodeDef*) SachiNode_GetDefition(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Defition;
}

SACHI_PUBLIC(int) SachiNode_SetDefition(Sachi_Object* InObject, Sachi_NodeDef* InDefinition)
{
	Sachi_Node* Node = (Sachi_Node*)InObject;

	Node->Defition = InDefinition;

	// Instantiate pins
	SachiList_Clear(Node->Pins);
	Sachi_PinDef* PinDef = InDefinition->Pins;
	while (PinDef && PinDef->Name != NULL)
	{
		Sachi_Object* Pin = Sachi_NewPin(Node->Interpreter);
		if (!Pin)
		{
			return SACHI_ERROR;
		}
		SachiPin_SetDefition(Pin, PinDef);
		if (SachiList_Push(Node->Pins, Pin) != SACHI_OK)
		{
			Sachi_DecRef(Pin);
			return SACHI_ERROR;
		}
		Sachi_DecRef(Pin);
		PinDef++;
	}

	// Instantiate children
	SachiList_Clear(Node->Children);
	Sachi_NodeDef* ChildDef = InDefinition->Children;
	while (ChildDef && ChildDef->Name != NULL)
	{
		Sachi_Node* Child = Sachi_NewNode(Node->Interpreter);
		if (!Child)
		{
			return SACHI_ERROR;
		}
		SachiNode_SetDefition(Child, ChildDef);
		if (SachiList_Push(Node->Children, Child) != SACHI_OK)
		{
			Sachi_DecRef(Child);
			return SACHI_ERROR;
		}
		Sachi_DecRef(Child);
		ChildDef++;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(const char*) SachiNode_GetName(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Defition->Name;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPins(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Pins;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPin(Sachi_Object* InObject, const char* InName)
{
	Sachi_Object* List = ((Sachi_Node*)InObject)->Pins;

	Sachi_Object** Items = SachiList_Data(List);
	sachi_size_t Size = SachiList_Size(Items);
	for (sachi_size_t I = 0; I < Size; ++I)
	{
		if (sachi_strcmp(SachiPin_GetName(*Items), InName) == 0)
		{
			return *Items;
		}
		Items++;
	}

	return NULL;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetChildren(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Children;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetChild(Sachi_Object* InObject, const char* InName)
{
	Sachi_Object* List = ((Sachi_Node*)InObject)->Children;

	Sachi_Object** Items = SachiList_Data(List);
	sachi_size_t Size = SachiList_Size(List);
	for (sachi_size_t I = 0; I < Size; ++I)
	{
		const char* Name = SachiNode_GetName(*Items);
		if (sachi_strcmp(Name, InName) == 0)
		{
			return *Items;
		}
		Items++;
	}

	return NULL;
}

SACHI_PUBLIC(int) SachiNode_Call(Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults)
{
	Sachi_NodeDef* OutDefinition = SachiNode_GetDefition(InObject);
	if (!OutDefinition)
	{
		return SACHI_ERROR;
	}

	return OutDefinition->Func(
		InObject->Interpreter,
		InObject,
		InInputExecPin,
		InKwArgs,
		OutOutputExecPin,
		OutKwResults
	);
}