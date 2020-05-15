#include "sachi/object/int.h"
#include "sachi/sachi.h"
#include "sachi/interpreter.h"
#include "sachi/object/node.h"
#include "sachi/object/bool.h"

typedef struct _Sachi_Int
{
	SACHI_OBJECT_HEADER
	sachi_ssize_t Value;
} Sachi_Int;

static void _Sachi_DeleteInt(Sachi_Object* InObject)
{
	Sachi_DeleteInt(InObject);
}

Sachi_ObjectType Sachi_IntType = {
	"int",
	NULL, // base
	NULL, // new
	_Sachi_DeleteInt,
	NULL, // nodes
	NULL, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewInt(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewIntFromValue(InInterpreter, 0);
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewIntFromValue(Sachi_Interpreter* InInterpreter, sachi_ssize_t InValue)
{
	Sachi_Int* Value = (Sachi_Int*)sachi_malloc(sizeof(Sachi_Int));
	if (!Value)
	{
		SachiError_SetMemoryAllocation();
		return NULL;
	}

	Sachi_NewObject(InInterpreter, Value, &Sachi_IntType);
	Value->Value = InValue;

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(void) Sachi_DeleteInt(Sachi_Object* InObject)
{
	Sachi_Int* Int = (Sachi_Int*)InObject;
	Int->Value = 0;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(sachi_ssize_t) SachiInt_Data(Sachi_Object* InObject)
{
	return ((Sachi_Int*)InObject)->Value;
}