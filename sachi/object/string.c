#include "sachi/object/string.h"
#include "sachi/sachi.h"
#include "sachi/object/interpreter.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"

typedef struct _Sachi_String
{
	SACHI_OBJECT_HEADER
	const char* Buffer;
	sachi_size_t Size;
} Sachi_String;

static void _Sachi_DeleteString(Sachi_Object* InObject)
{
	Sachi_DeleteString(InObject);
}

static int _SachiString_Init(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults)
{
	return SACHI_OK;
}

static LONG _SachiString_Hash(Sachi_Object* InObject)
{
	Sachi_String* String = (Sachi_String*)InObject;
	return Sachi_HashFromBufferAndLength(String->Buffer, String->Size);
}

static Sachi_NodeDef _Sachi_StringNodes[] = {
	{"init", &_SachiString_Init}
};

Sachi_ObjectType Sachi_StringType = {
	"string",
	sizeof(Sachi_String),
	NULL, // base
	NULL, // new
	_Sachi_DeleteString,
	_Sachi_StringNodes,
	_SachiString_Hash, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewString(Sachi_Interpreter* InInterpreter)
{
	Sachi_String* Value = (Sachi_String*)Sachi_NewObject(InInterpreter, &Sachi_StringType);
	if (!Value)
	{
		return NULL;
	}
 
	Value->Buffer = NULL;
	Value->Size = 0;

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewStringFromBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer)
{
	return Sachi_NewStringFromBufferAndLength(InInterpreter, InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewStringFromBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength)
{
	Sachi_String* String = (Sachi_String*)Sachi_NewString(InInterpreter);
	if (!String)
	{
		return NULL;
	}

	char* Buffer = (char*)sachi_malloc(sizeof(char) * (InLength + 1));
	if (!Buffer)
	{
		SachiInterpreter_MemoryAllocationError(InInterpreter);
		Sachi_DeleteString(String);
		return NULL;
	}

	String->Buffer = Buffer;
	String->Size = InLength;
	sachi_memcpy(Buffer, InBuffer, InLength);
	Buffer[InLength] = '\0';

	return (Sachi_Object*)String;
}

SACHI_PUBLIC(void) Sachi_DeleteString(Sachi_Object* InObject)
{
	Sachi_String* String = (Sachi_String*)InObject;
	sachi_free(String->Buffer);
	String->Buffer = NULL;
	String->Size = 0;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(Sachi_Object*) SachiString_Empty(Sachi_Object* InObject)
{
	if (((Sachi_String*)InObject)->Size == 0)
	{
		return Sachi_True;
	}

	return Sachi_False;
}

SACHI_PUBLIC(sachi_size_t) SachiString_Size(Sachi_Object* InObject)
{
	return ((Sachi_String*)InObject)->Size;
}

SACHI_PUBLIC(const char*) SachiString_Data(Sachi_Object* InObject)
{
	return ((Sachi_String*)InObject)->Buffer;
}