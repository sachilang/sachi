#include "sachi/object/json/jsonnode.h"
#include "sachi/object/json/jsonpin.h"
#include "sachi/object/node.h"
#include "sachi/object/nodeinstance.h"
#include "sachi/object/pin.h"
#include "sachi/object/string.h"
#include "sachi/object/list.h"
#include "sachi/object/dict.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"
#include "sachi/sachi.h"

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromJSON(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict)
{
	Sachi_Object* Node = Sachi_NewNode(InInterpreter);
	if (!Node)
	{
		return NULL;
	}

	if (SachiNode_InitFromJSON(Node, InDict) != SACHI_OK)
	{
		Sachi_DecRef(Node);
		return NULL;
	}

	return Node;
}

static int _SachiNode_InitPinsFromJSON(Sachi_Object* InObject, Sachi_Object* InList)
{
	sachi_size_t Size = SachiList_Size(InList);
	Sachi_Object* Pins = Sachi_NewListWithCapacity(InObject->Interpreter, Size);
	if (!Pins)
	{
		return SACHI_ERROR;
	}

	SACHILIST_FOREACH(InList, Index, Items)
	{
		Sachi_Object* Pin = Sachi_NewPinFromJSON(InObject->Interpreter, *Items);
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
	}

	if (SachiNode_SetPins(InObject, Pins) != SACHI_OK)
	{
		Sachi_DecRef(Pins);
		return SACHI_ERROR;
	}

	Sachi_DecRef(Pins);
	return SACHI_OK;
}

static Sachi_Object* _Sachi_NewNodeInstanceFromJSON(Sachi_Object* InObject, Sachi_Object* InList)
{
	return Sachi_NewNodeInstance(InObject);
}

static int _SachiNode_InitFlowFromJSON(Sachi_Object* InObject, Sachi_Object* InList)
{
	sachi_size_t Size = SachiList_Size(InList);
	Sachi_Object* Flow = Sachi_NewListWithCapacity(InObject->Interpreter, Size);
	if (!Flow)
	{
		return SACHI_ERROR;
	}

	SACHILIST_FOREACH(InList, Index, Items)
	{
		Sachi_Object* NodeInstance = _Sachi_NewNodeInstanceFromJSON(InObject->Interpreter, *Items);
		if (!NodeInstance)
		{
			Sachi_DecRef(Flow);
			return SACHI_ERROR;
		}

		if (SachiList_Push(Flow, NodeInstance) != SACHI_OK)
		{
			Sachi_DecRef(NodeInstance);
			Sachi_DecRef(Flow);
			return SACHI_ERROR;
		}
		Sachi_DecRef(NodeInstance);
	}

	SachiNode_SetFlow(InObject, Flow);
	Sachi_DecRef(Flow);
	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNode_InitFromJSON(Sachi_Object* InObject, Sachi_Object* InDict)
{
	Sachi_Object* Name = NULL;
	Sachi_Object* Pins = NULL;
	Sachi_Object* Flow = NULL;
	if (SachiDict_GetItemFromBuffer(InDict, "name", &Name) != SACHI_OK ||
		SachiDict_GetItemFromBuffer(InDict, "pins", &Pins) != SACHI_OK ||
		SachiDict_GetItemFromBuffer(InDict, "flow", &Flow) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!Sachi_IsString(Name))
	{
		SachiInterpreter_SetErrorMessage(InObject, "invalid type for node name, string expected");
		return SACHI_ERROR;
	}

	if (SachiNode_SetName(InObject, Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!Sachi_IsList(Pins))
	{
		SachiInterpreter_SetErrorMessage(InObject, "invalid type for node pins, list expected");
		return SACHI_ERROR;
	}

	if (_SachiNode_InitPinsFromJSON(InObject, Pins) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!Sachi_IsList(Flow))
	{
		SachiInterpreter_SetErrorMessage(InObject, "invalid type for node flow, list expected");
		return SACHI_ERROR;
	}

	if (_SachiNode_InitFlowFromJSON(InObject, Flow) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}