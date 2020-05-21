#include "sachi/object/metadata/metadatanode.h"
#include "sachi/object/metadata/metadatapin.h"
#include "sachi/object/node.h"
#include "sachi/object/pin.h"
#include "sachi/object/string.h"
#include "sachi/object/list.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromMetadata(Sachi_Interpreter* InInterpreter, Sachi_NodeMetadata* InMetadata)
{
	Sachi_Object* Value = Sachi_NewNode(InInterpreter);
	if (Value)
	{
		SachiNode_IntFromMetadata(Value, InMetadata);
	}

	return Value;
}

SACHI_PUBLIC(int) SachiNode_IntFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata)
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

SACHI_PUBLIC(int) SachiNode_SetPinsFromMetadata(Sachi_Object* InObject, Sachi_PinMetadata* InPins)
{
	Sachi_Object* Pins = Sachi_NewList(InObject->Interpreter);
	if (!Pins)
	{
		return SACHI_ERROR;
	}

	while(InPins && InPins->Name)
	{
		Sachi_Object* Pin = Sachi_NewPinFromMetadata(InObject->Interpreter, InPins);
		if (!Pin)
		{
			Sachi_DecRef(Pins);
			return SACHI_ERROR;
		}

		if (SachiList_Push(Pins, Pin) != SACHI_OK)
		{
			Sachi_DecRef(Pin);
			Sachi_DecRef(Pins);
			return SACHI_ERROR;
		}
		Sachi_DecRef(Pin);
		InPins++;
	}

	if (SachiNode_SetPins(InObject, Pins) != SACHI_OK)
	{
		Sachi_DecRef(Pins);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Pins);
	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNode_SetNodesFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InNodes)
{
	Sachi_Object* Nodes = Sachi_NewList(InObject->Interpreter);
	if (!Nodes)
	{
		return SACHI_ERROR;
	}

	while (InNodes && InNodes->Name)
	{
		Sachi_Object* Child = Sachi_NewNodeFromMetadata(InObject->Interpreter, InNodes);
		if (!Child)
		{
			Sachi_DecRef(Nodes);
			return SACHI_ERROR;
		}

		if (SachiList_Push(Nodes, Child) != SACHI_OK)
		{
			Sachi_DecRef(Child);
			Sachi_DecRef(Nodes);
			return SACHI_ERROR;
		}
		Sachi_DecRef(Child);
		InNodes++;
	}

	if (SachiNode_SetNodes(InObject, Nodes) != SACHI_OK)
	{
		Sachi_DecRef(Nodes);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Nodes);
	return SACHI_OK;
}