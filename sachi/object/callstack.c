#include "sachi/object/callstack.h"
#include "sachi/object/interpreter.h"
#include "sachi/sachi.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"
#include "sachi/object/dict.h"

typedef struct _Sachi_CallStack
{
	SACHI_OBJECT_HEADER
	Sachi_Object* Parent; // parent callstack
	Sachi_Object* Nodes; // known nodes accessibles from code
} Sachi_CallStack;

static void _Sachi_DeleteCallStack(Sachi_Object* InObject)
{
	Sachi_DeleteCallStack(InObject);
}

Sachi_ObjectType Sachi_CallStackType = {
	"callstack",
	sizeof(Sachi_CallStack),
	NULL, // base
	NULL, // new
	_Sachi_DeleteCallStack,
	NULL, // nodes
	NULL, // hash
	NULL, // to string
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStack(Sachi_Interpreter* InInterpreter, Sachi_Object* InParent)
{
	Sachi_CallStack* Value = (Sachi_CallStack*)Sachi_NewObject(InInterpreter, &Sachi_CallStackType);
	if (!Value)
	{
		return NULL;
	}

	Value->Parent = InParent;
	Sachi_IncRef(InParent);
	Value->Nodes = NULL;

	Value->Nodes = Sachi_NewDict(InInterpreter);
	if (!Value->Nodes)
	{
		goto fail;
	}

	return (Sachi_Object*)Value;

fail:
	Sachi_DeleteCallStack(Value);
	return NULL;
}

SACHI_PUBLIC(void) Sachi_DeleteCallStack(Sachi_Object* InObject)
{
	Sachi_CallStack* CallStack = (Sachi_CallStack*)InObject;
	Sachi_DecRef(CallStack->Parent);
	CallStack->Parent = NULL;
	Sachi_DecRef(CallStack->Nodes);
	CallStack->Nodes = NULL;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(int) SachiCallStack_AddNode(Sachi_Object* InObject, Sachi_Object* InNode)
{
	Sachi_CallStack* CallStack = (Sachi_CallStack*)InObject;
	return SachiDict_SetItemFromBuffer(CallStack->Nodes, SachiNode_GetName(InNode), InNode);
}

SACHI_PUBLIC(int) SachiCallStack_AddNodeFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata)
{
	Sachi_Object* Node = Sachi_NewNodeFromMetadata(InObject->Interpreter, InMetadata);
	if (!Node)
	{
		return SACHI_ERROR;
	}

	int Result = SachiCallStack_AddNode(InObject, Node);
	Sachi_DecRef(Node);
	return Result;
}

SACHI_PUBLIC(int) SachiCallStack_AddNodeFromDict(Sachi_Object* InObject, Sachi_Object* InDict)
{
	Sachi_Object* Node = Sachi_NewNodeFromDict(InObject->Interpreter, InDict);
	if (!Node)
	{
		return SACHI_ERROR;
	}

	int Result = SachiCallStack_AddNode(InObject, Node);
	Sachi_DecRef(Node);
	return Result;
}