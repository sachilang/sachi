#include "sachi/object/nodeinstance.h"
#include "sachi/object/interpreter.h"
#include "sachi/object/callstack.h"
#include "sachi/object/dict.h"
#include "sachi/object/node.h"
#include "sachi/pinmetadata.h"
#include "sachi/nodemetadata.h"
#include "sachi/sachi.h"

typedef struct _Sachi_NodeInstance
{
	SACHI_OBJECT_HEADER
	Sachi_Object* CallStack; // callstack allocated to this instance
	Sachi_Object* Node; // node used to create this instance
	Sachi_Object* InputExecPin; // from where we entered this node
	Sachi_Object* OutputExecPin; // from where we exit this node
	Sachi_Object* KwArgs; // arguments received
	Sachi_Object* KwResults; // results produced
	Sachi_Object* Locals; // local variables
} Sachi_NodeInstance;

static Sachi_Object* _Sachi_NewNodeInstance(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewNodeInstance(InInterpreter);
}

static void _Sachi_DeleteNodeInstance(Sachi_Object* InObject)
{
	Sachi_DeleteNodeInstance(InObject);
}

Sachi_ObjectType Sachi_NodeInstanceType = {
	"nodeinstance",
	sizeof(Sachi_NodeInstance),
	NULL, // base
	_Sachi_NewNodeInstance,
	_Sachi_DeleteNodeInstance,
	NULL, // nodes
	NULL, // hash
	NULL, // to string
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeInstance(Sachi_Object* InNode)
{
	Sachi_NodeInstance* Value = (Sachi_NodeInstance*)Sachi_NewObject(InNode->Interpreter, &Sachi_NodeInstanceType);
	if (!Value)
	{
		return NULL;
	}

	Value->CallStack = NULL;
	Value->Node = InNode;
	Sachi_IncRef(InNode);
	Value->InputExecPin = NULL;
	Value->OutputExecPin = NULL;
	Value->KwArgs = NULL;
	Value->KwResults = NULL;

	Value->KwArgs = Sachi_NewDict(InNode->Interpreter);
	if (!Value->KwArgs)
	{
		goto fail;
	}

	Value->KwResults = Sachi_NewDict(InNode->Interpreter);
	if (!Value->KwResults)
	{
		goto fail;
	}

	return (Sachi_Object*)Value;

fail:
	Sachi_DeleteNodeInstance((Sachi_Object*)Value);
	return NULL;
}

SACHI_PUBLIC(void) Sachi_DeleteNodeInstance(Sachi_Object* InObject)
{
	if (!InObject)
	{
		return;
	}

	Sachi_NodeInstance* NodeInstance = (Sachi_NodeInstance*)InObject;
	Sachi_DecRef(NodeInstance->CallStack);
	NodeInstance->CallStack = NULL;
	Sachi_DecRef(NodeInstance->Node);
	NodeInstance->Node = NULL;
	Sachi_DecRef(NodeInstance->InputExecPin);
	NodeInstance->InputExecPin = NULL;
	Sachi_DecRef(NodeInstance->OutputExecPin);
	NodeInstance->OutputExecPin = NULL;
	Sachi_DecRef(NodeInstance->KwArgs);
	NodeInstance->KwArgs = NULL;
	Sachi_DecRef(NodeInstance->KwResults);
	NodeInstance->KwResults = NULL;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetNode(const Sachi_Object* InObject)
{
	return ((const Sachi_NodeInstance*)InObject)->Node;
}

SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetInputExecPin(const Sachi_Object* InObject)
{
	return ((const Sachi_NodeInstance*)InObject)->InputExecPin;
}

SACHI_PUBLIC(void) SachiNodeInstance_SetInputExecPin(Sachi_Object* InObject, Sachi_Object* InPin)
{
	Sachi_NodeInstance* NodeInstance = (Sachi_NodeInstance*)InObject;
	Sachi_Object* Old = NodeInstance->InputExecPin;
	NodeInstance->InputExecPin = InPin;
	Sachi_IncRef(InPin);
	Sachi_DecRef(Old);
}

SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetKwArgs(const Sachi_Object* InObject)
{
	return ((const Sachi_NodeInstance*)InObject)->KwArgs;
}

SACHI_PUBLIC(void) SachiNodeInstance_SetKwArgs(Sachi_Object* InObject, Sachi_Object* InKwArgs)
{
	Sachi_NodeInstance* NodeInstance = (Sachi_NodeInstance*)InObject;
	Sachi_Object* Old = NodeInstance->KwArgs;
	NodeInstance->KwArgs = InKwArgs;
	Sachi_IncRef(InKwArgs);
	Sachi_DecRef(Old);
}

SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetOutputExecPin(const Sachi_Object* InObject)
{
	return ((const Sachi_NodeInstance*)InObject)->OutputExecPin;
}

SACHI_PUBLIC(const Sachi_Object*) SachiNodeInstance_GetKwResults(const Sachi_Object* InObject)
{
	return ((const Sachi_NodeInstance*)InObject)->KwResults;
}

SACHI_PUBLIC(int) SachiNodeInstance_Call(Sachi_Object* InObject, Sachi_Object* InCallStack)
{
	Sachi_NodeInstance* NodeInstance = (Sachi_NodeInstance*)InObject;

	Sachi_DecRef(NodeInstance->CallStack);
	NodeInstance->CallStack = Sachi_NewCallStack(InObject->Interpreter, InCallStack);
	if (!NodeInstance->CallStack)
	{
		return SACHI_ERROR;
	}

	if (SachiNode_GetFunc(NodeInstance->Node)(
		InObject,
		NodeInstance->InputExecPin,
		NodeInstance->KwArgs,
		&NodeInstance->OutputExecPin,
		NodeInstance->KwResults
		) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_DecRef(NodeInstance->CallStack);
	NodeInstance->CallStack = NULL;

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNodeInstance_CallWithArgs(Sachi_Object* InObject, Sachi_Object* InCallStack, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	Sachi_NodeInstance* NodeInstance = (Sachi_NodeInstance*)InObject;

	SachiNodeInstance_SetInputExecPin(InObject, InInputExecPin);
	SachiNodeInstance_SetKwArgs(InObject, InKwArgs);

	Sachi_DecRef(NodeInstance->OutputExecPin);
	NodeInstance->OutputExecPin = NULL;

	Sachi_DecRef(NodeInstance->KwResults);
	NodeInstance->KwResults = Sachi_NewDict(InObject->Interpreter);
	if (!NodeInstance->KwResults)
	{
		return SACHI_ERROR;
	}

	if (SachiNodeInstance_Call(InObject, InCallStack) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_IncRef(NodeInstance->OutputExecPin);

	if (OutOutputExecPin)
	{
		*OutOutputExecPin = NodeInstance->OutputExecPin;
	}

	if (OutKwResults)
	{
		*OutKwResults = NodeInstance->KwResults;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiNodeInstance_SetLocal(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object* InValue)
{
	return SachiDict_SetItem(((Sachi_NodeInstance*)InObject)->Locals, InKey, InValue);
}

SACHI_PUBLIC(int) SachiNodeInstance_SetLocalFromBuffer(Sachi_Object* InObject, const char* InBuffer, Sachi_Object* InValue)
{
	return SachiDict_SetItemFromBuffer(((Sachi_NodeInstance*)InObject)->Locals, InBuffer, InValue);
}

SACHI_PUBLIC(int) SachiNodeInstance_SetLocalFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength, Sachi_Object* InValue)
{
	return SachiDict_SetItemFromBufferAndLength(((Sachi_NodeInstance*)InObject)->Locals, InBuffer, InLength, InValue);
}

SACHI_PUBLIC(int) SachiNodeInstance_GetLocal(Sachi_Object* InObject, Sachi_Object* InKey, Sachi_Object** OutValue)
{
	return SachiDict_GetItem(((Sachi_NodeInstance*)InObject)->Locals, InKey, OutValue);
}

SACHI_PUBLIC(int) SachiNodeInstance_GetLocalFromBuffer(Sachi_Object* InObject, const char* InBuffer, Sachi_Object** OutValue)
{
	return SachiDict_GetItemFromBuffer(((Sachi_NodeInstance*)InObject)->Locals, InBuffer, OutValue);
}

SACHI_PUBLIC(int) SachiNodeInstance_GetLocalFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutValue)
{
	return SachiDict_GetItemFromBufferAndLength(((Sachi_NodeInstance*)InObject)->Locals, InBuffer, InLength, OutValue);
}