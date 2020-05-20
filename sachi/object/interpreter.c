#include "sachi/object/interpreter.h"
#include "sachi/sachi.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"
#include "sachi/object/callstack.h"
#include "sachi/node/builtin.h"

typedef struct _Sachi_Interpreter
{
	SACHI_OBJECT_HEADER
	Sachi_CallStack* CallStack; // root of callstack
	Sachi_Object* True; // true boolean
	Sachi_Object* False; // false boolean
	struct {
		const char* Message;
	} Error; // last error if any
} Sachi_Interpreter;

static Sachi_Object* _Sachi_NewInterpreter(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewInterpreter(InInterpreter);
}

static void _Sachi_DeleteInterpreter(Sachi_Object* InObject)
{
	Sachi_DeleteInterpreter(InObject);
}

Sachi_ObjectType Sachi_InterpreterType = {
	"interpreter",
	sizeof(Sachi_Interpreter),
	NULL, // base
	_Sachi_NewInterpreter,
	_Sachi_DeleteInterpreter,
	NULL, // nodes
	NULL, // hash,
};

static int _SachiInterpreter_InitNodes(Sachi_Object* InObject)
{
	Sachi_NodeMetadata** Metadata = &Sachi_BuiltinNodesMetadata;
	while (*Metadata)
	{
		if (SachiInterpreter_AddNodeFromMetadata(InObject, &Sachi_ImportNodeMetadata) != SACHI_OK)
		{
			return SACHI_ERROR;
		}
		Metadata++;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewInterpreter(Sachi_Interpreter* InInterpreter)
{
	Sachi_Interpreter* Value = (Sachi_Interpreter*)Sachi_NewObject(InInterpreter, &Sachi_InterpreterType);
	if (!Value)
	{
		return NULL;
	}

	Value->CallStack = NULL;
	Value->True = NULL;
	Value->False = NULL;
	Value->Error.Message = NULL;

	Value->CallStack = Sachi_NewCallStack(Value, NULL);
	if (!Value->CallStack)
	{
		goto fail;
	}

	Value->True = Sachi_NewBool(Value);
	if (!Value->True)
	{
		goto fail;
	}

	Value->False = Sachi_NewBool(Value);
	if (!Value->False)
	{
		goto fail;
	}

	if (_SachiInterpreter_InitNodes(Value) != SACHI_OK)
	{
		goto fail;
	}

	return Value;

fail:
	Sachi_DeleteInterpreter(Value);
	return NULL;
}

SACHI_PUBLIC(void) Sachi_DeleteInterpreter(Sachi_Object* InObject)
{
	Sachi_Interpreter* Interpreter = (Sachi_Interpreter*)InObject;
	Sachi_DecRef(Interpreter->CallStack);
	Interpreter->CallStack = NULL;
	Sachi_DeleteBool(Interpreter->True);
	Interpreter->True = NULL;
	Sachi_DeleteBool(Interpreter->False);
	Interpreter->False = NULL;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(const char*) SachiInterpreter_GetErrorMessage(Sachi_Object* InObject)
{
	return ((Sachi_Interpreter*)InObject)->Error.Message;
}

SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessage(Sachi_Object* InObject, const char* InBuffer)
{
	((Sachi_Interpreter*)InObject)->Error.Message = InBuffer;
}

SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessageWithLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InBufferLength)
{
	((Sachi_Interpreter*)InObject)->Error.Message = InBuffer;
}

SACHI_PUBLIC(void) SachiInterpreter_MemoryAllocationError(Sachi_Object* InObject)
{
	SachiInterpreter_SetErrorMessage(InObject, "memory allocation failed");
}

SACHI_PUBLIC(int) SachiInterpreter_AddNode(Sachi_Object* InObject, Sachi_Object* InNode)
{
	return SachiCallStack_AddNode(((Sachi_Interpreter*)InObject)->CallStack, InNode);
}

SACHI_PUBLIC(int) SachiInterpreter_AddNodeFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata)
{
	return SachiCallStack_AddNodeFromMetadata(((Sachi_Interpreter*)InObject)->CallStack, InMetadata);
}

SACHI_PUBLIC(int) SachiInterpreter_AddNodeFromDict(Sachi_Object* InObject, Sachi_Object* InDict)
{
	return SachiCallStack_AddNodeFromDict(((Sachi_Interpreter*)InObject)->CallStack, InDict);
}

SACHI_PUBLIC(Sachi_Object*) Sachi_True(Sachi_Object* InInterpreter)
{
	return ((Sachi_Interpreter*)InInterpreter)->True;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_False(Sachi_Object* InInterpreter)
{
	return ((Sachi_Interpreter*)InInterpreter)->False;
}