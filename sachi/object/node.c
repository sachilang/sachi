#include "sachi/object/node.h"
#include "sachi/object/pin.h"
#include "sachi/object/string.h"
#include "sachi/object/list.h"
#include "sachi/object/dict.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"
#include "sachi/sachi.h"

typedef struct _Sachi_Node
{
	SACHI_OBJECT_HEADER
	const char* Name;
	Sachi_CFunc Func;
	Sachi_Object* Pins;
	Sachi_Object* Nodes;
} Sachi_Node;

static Sachi_Object* _Sachi_NewNode(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewNode(InInterpreter);
}

static void _Sachi_DeleteNode(Sachi_Object* InObject)
{
	Sachi_DeleteNode(InObject);
}

Sachi_ObjectType Sachi_NodeType = {
	"node",
	sizeof(Sachi_Node),
	NULL, // base
	_Sachi_NewNode,
	_Sachi_DeleteNode,
	NULL, // nodes
	NULL, // hash
	NULL, // to string
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNode(Sachi_Interpreter* InInterpreter)
{
	Sachi_Node* Value = (Sachi_Node*)Sachi_NewObject(InInterpreter, &Sachi_NodeType);
	if (!Value)
	{
		return NULL;
	}

	Value->Name = NULL;
	Value->Func = NULL;
	Value->Pins = NULL;
	Value->Nodes = NULL;

	Value->Pins = Sachi_NewList(InInterpreter);
	if (!Value->Pins)
	{
		Sachi_DeleteNode(Value);
		return NULL;
	}

	Value->Nodes = Sachi_NewList(InInterpreter);
	if (!Value->Nodes)
	{
		Sachi_DeleteNode(Value);
		return NULL;
	}

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromMetadata(Sachi_Interpreter* InInterpreter, Sachi_NodeMetadata* InMetadata)
{
	Sachi_Object* Value = Sachi_NewNode(InInterpreter);
	if (Value)
	{
		SachiNode_SetMetadata(Value, InMetadata);
	}

	return Value;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromDict(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict)
{
	Sachi_Object* Value = Sachi_NewNode(InInterpreter);
	if (Value)
	{
		SachiNode_SetMetadataFromDict(Value, InDict);
	}

	return Value;
}

SACHI_PUBLIC(void) Sachi_DeleteNode(Sachi_Object* InObject)
{
	if (!InObject)
	{
		return;
	}

	Sachi_Node* Node = (Sachi_Node*)InObject;

	sachi_free(Node->Name);
	Node->Name = NULL;
	Node->Func = NULL;
	Sachi_DecRef(Node->Pins);
	Node->Pins = NULL;
	Sachi_DecRef(Node->Nodes);
	Node->Nodes = NULL;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(int) SachiNode_SetMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata)
{
	if (SachiNode_SetNameFromBuffer(InObject, InMetadata->Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	SachiNode_SetFunc(InObject, InMetadata->Func);

	if (SachiNode_SetPinsFromMetadata(InObject, InMetadata->Pins) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiNode_SetNodesFromMetadata(InObject, InMetadata->Nodes) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNode_SetMetadataFromDict(Sachi_Object* InObject, Sachi_Object* InDict)
{
	Sachi_Object* Name = NULL;
	Sachi_Object* Pins = NULL;
	Sachi_Object* Nodes = NULL;

	if (SachiDict_GetItemFromBuffer(InDict, "name", &Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_GetItemFromBuffer(InDict, "pins", &Pins) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_GetItemFromBuffer(InDict, "nodes", &Nodes) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiNode_SetName(InObject, Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiNode_SetPins(InObject, Pins) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiNode_SetNodes(InObject, Nodes) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(const char*) SachiNode_GetName(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Name;
}

SACHI_PUBLIC(int) SachiNode_SetName(Sachi_Object* InObject, Sachi_Object* InName)
{
	return SachiNode_SetNameFromBufferAndLength(InObject, SachiString_Data(InName), SachiString_Size(InName));
}

SACHI_PUBLIC(int) SachiNode_SetNameFromBuffer(Sachi_Object* InObject, const char* InBuffer)
{
	return SachiNode_SetNameFromBufferAndLength(InObject, InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(int) SachiNode_SetNameFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength)
{
	const char* Buffer = NULL;
	if (Sachi_CopyBuffer(InBuffer, &Buffer) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_Node* Node = (Sachi_Node*)InObject;
	sachi_free(Node->Name);
	Node->Name = Buffer;

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_CFunc) SachiNode_GetFunc(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Func;
}

SACHI_PUBLIC(void) SachiNode_SetFunc(Sachi_Object* InObject, Sachi_CFunc InFunc)
{
	((Sachi_Node*)InObject)->Func = InFunc;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPins(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Pins;
}

SACHI_PUBLIC(int) SachiNode_SetPins(Sachi_Object* InObject, Sachi_Object* InPins)
{
	Sachi_Node* Node = (Sachi_Node*)InObject;

	SachiList_Clear(Node->Pins);
	if (!InPins)
	{
		return SACHI_OK;
	}

	Sachi_Object** Items = SachiList_Data(InPins);
	sachi_size_t Size = SachiList_Size(InPins);
	for (sachi_size_t I = 0; I < Size; ++I)
	{
		if (SachiList_Push(Node->Pins, *Items) != SACHI_OK)
		{
			return SACHI_ERROR;
		}
		Items++;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNode_SetPinsFromMetadata(Sachi_Object* InObject, Sachi_PinMetadata* InPins)
{
	Sachi_Node* Node = (Sachi_Node*)InObject;

	SachiList_Clear(Node->Pins);
	while(InPins && InPins->Name)
	{
		Sachi_Object* Pin = Sachi_NewPinFromMetadata(InObject->Interpreter, InPins);
		if (!Pin)
		{
			return SACHI_ERROR;
		}

		if (SachiList_Push(Node->Pins, Pin) != SACHI_OK)
		{
			Sachi_DecRef(Pin);
			return SACHI_ERROR;
		}
		Sachi_DecRef(Pin);
		InPins++;
	}

	return SACHI_OK;
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

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetNodes(Sachi_Object* InObject)
{
	return ((Sachi_Node*)InObject)->Nodes;
}

SACHI_PUBLIC(int) SachiNode_SetNodes(Sachi_Object* InObject, Sachi_Object* InNodes)
{
	Sachi_Node* Node = (Sachi_Node*)InObject;

	SachiList_Clear(Node->Nodes);
	if (!InNodes)
	{
		return SACHI_OK;
	}

	Sachi_Object** Items = SachiList_Data(InNodes);
	sachi_size_t Size = SachiList_Size(InNodes);
	for (sachi_size_t I = 0; I < Size; ++I)
	{
		if (SachiList_Push(Node->Nodes, *Items) != SACHI_OK)
		{
			return SACHI_ERROR;
		}
		Items++;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNode_SetNodesFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InNodes)
{
	Sachi_Node* Node = (Sachi_Node*)InObject;

	SachiList_Clear(Node->Nodes);
	while (InNodes && InNodes->Name)
	{
		Sachi_Object* Child = Sachi_NewNodeFromMetadata(InObject->Interpreter, InNodes);
		if (!Child)
		{
			return SACHI_ERROR;
		}

		if (SachiList_Push(Node->Nodes, Child) != SACHI_OK)
		{
			Sachi_DecRef(Child);
			return SACHI_ERROR;
		}
		Sachi_DecRef(Child);
		InNodes++;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) SachiNode_GetNode(Sachi_Object* InObject, const char* InName)
{
	Sachi_Object* List = ((Sachi_Node*)InObject)->Nodes;

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
	Sachi_CFunc Func = SachiNode_GetFunc(InObject);
	if (!Func)
	{
		return SACHI_ERROR;
	}

	return Func(
		InObject->Interpreter,
		InObject,
		InInputExecPin,
		InKwArgs,
		OutOutputExecPin,
		OutKwResults
	);
}