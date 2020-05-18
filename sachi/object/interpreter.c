#include "sachi/object/interpreter.h"
#include "sachi/sachi.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"
#include "sachi/object/callstack.h"

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

	Value->True = Sachi_NewBool(InInterpreter);
	if (!Value->True)
	{
		Sachi_DeleteInterpreter(Value);
		return NULL;
	}

	Value->False = Sachi_NewBool(InInterpreter);
	if (!Value->False)
	{
		Sachi_DeleteInterpreter(Value);
		return NULL;
	}

	return Value;
}

SACHI_PUBLIC(void) Sachi_DeleteInterpreter(Sachi_Object* InObject)
{
	Sachi_Interpreter* Interpreter = (Sachi_Interpreter*)InObject;
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

SACHI_PUBLIC(Sachi_Object*) Sachi_True(Sachi_Object* InInterpreter)
{
	return ((Sachi_Interpreter*)InInterpreter)->True;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_False(Sachi_Object* InInterpreter)
{
	return ((Sachi_Interpreter*)InInterpreter)->False;
}