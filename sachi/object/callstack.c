#include "sachi/object/callstack.h"
#include "sachi/object/interpreter.h"
#include "sachi/sachi.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"
#include "sachi/nodemetadata.h"

typedef struct _Sachi_CallStack
{
	SACHI_OBJECT_HEADER
	const char* Buffer;
	sachi_size_t Size;
} Sachi_CallStack;

static void _Sachi_DeleteCallStack(Sachi_Object* InObject)
{
	Sachi_DeleteCallStack(InObject);
}

static int _SachiCallStack_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static LONG _SachiCallStack_Hash(Sachi_Object* InObject)
{
	LONG result = 0x55555555;

	const char* Buffer = SachiCallStack_Data(InObject);
	while (*Buffer != '\0') {
		result ^= *Buffer++;
		result <<= 5;
	}

	return result;
}

static Sachi_NodeMetadata _Sachi_CallStackNodes[] = {
	{"init", &_SachiCallStack_Init}
};

Sachi_ObjectType Sachi_CallStackType = {
	"CallStack",
	NULL, // base
	NULL, // new
	_Sachi_DeleteCallStack,
	_Sachi_CallStackNodes,
	_SachiCallStack_Hash, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStack(Sachi_Interpreter* InInterpreter)
{
	Sachi_CallStack* Value = (Sachi_CallStack*)sachi_malloc(sizeof(Sachi_CallStack));
	if (!Value)
	{
		SachiError_SetMemoryAllocation();
		return NULL;
	}

	Sachi_NewObject(InInterpreter, Value, &Sachi_CallStackType);
	Value->Buffer = NULL;
	Value->Size = 0;

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStackFromBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer)
{
	return Sachi_NewCallStackFromBufferAndLength(InInterpreter, InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStackFromBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength)
{
	Sachi_CallStack* CallStack = (Sachi_CallStack*)Sachi_NewCallStack(InInterpreter);
	if (!CallStack)
	{
		return NULL;
	}

	char* Buffer = (char*)sachi_malloc(sizeof(char) * (InLength + 1));
	if (!Buffer)
	{
		SachiInterpreter_MemoryAllocationError(InInterpreter);
		Sachi_DeleteCallStack(CallStack);
		return NULL;
	}

	CallStack->Buffer = Buffer;
	CallStack->Size = InLength;
	sachi_memcpy(Buffer, InBuffer, InLength);
	Buffer[InLength] = '\0';

	return (Sachi_Object*)CallStack;
}

SACHI_PUBLIC(void) Sachi_DeleteCallStack(Sachi_Object* InObject)
{
	Sachi_CallStack* CallStack = (Sachi_CallStack*)InObject;
	sachi_free(CallStack->Buffer);
	CallStack->Buffer = NULL;
	CallStack->Size = 0;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(Sachi_Object*) SachiCallStack_Empty(Sachi_Object* InObject)
{
	if (((Sachi_CallStack*)InObject)->Size == 0)
	{
		return Sachi_True;
	}

	return Sachi_False;
}

SACHI_PUBLIC(sachi_size_t) SachiCallStack_Size(Sachi_Object* InObject)
{
	return ((Sachi_CallStack*)InObject)->Size;
}

SACHI_PUBLIC(const char*) SachiCallStack_Data(Sachi_Object* InObject)
{
	return ((Sachi_CallStack*)InObject)->Buffer;
}