#include "sachi/object/object.h"
#include "sachi/interpreter.h"

static LONG _SachiObject_Hash(Sachi_Object* InObject)
{
	return (sachi_size_t)InObject;
}

static Sachi_ObjectType _Sachi_ObjectType = {
	"object",
	NULL, // base
	NULL, // new
	NULL, // delete
	NULL, // nodes
	_SachiObject_Hash, // hash
};

SACHI_PUBLIC(int) Sachi_InitType(Sachi_ObjectType* InType)
{
	if (InType == &_Sachi_ObjectType)
	{
		return SACHI_OK;
	}

	if (!InType->Base)
	{
		InType->Base = &_Sachi_ObjectType;
	}

	if (!InType->Hash)
	{
		InType->Hash = InType->Base->Hash;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(void) Sachi_IncRef(Sachi_Object* InObject)
{
	if (!InObject)
	{
		return;
	}

	InObject->RefCounter.Counter++;
}

SACHI_PUBLIC(int) Sachi_DecRef(Sachi_Object* InObject)
{
	if (!InObject)
	{
		return SACHI_OK;
	}

	InObject->RefCounter.Counter--;
	if (InObject->RefCounter.Counter == 0)
	{
		if (!InObject->Type)
		{
			SachiInterpreter_SetErrorMessage(InObject->Interpreter, "object has no type");
			return SACHI_ERROR;
		}

		if (!InObject->Type->Delete)
		{
			SachiInterpreter_SetErrorMessage(InObject->Interpreter, "object has no destructor");
			return SACHI_ERROR;
		}

		InObject->Type->Delete(InObject);
	}

	return SACHI_OK;
}

SACHI_PUBLIC(void) Sachi_NewObject(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_ObjectType* InType)
{
	InObject->RefCounter.Counter = 0;
	InObject->Interpreter = InInterpreter;
	InObject->Type = InType;
}

SACHI_PUBLIC(void) Sachi_DeleteObject(Sachi_Object* InObject)
{
	InObject->RefCounter.Counter = 0;
	InObject->Interpreter = NULL;
	InObject->Type = NULL;
	sachi_free(InObject);
}

SACHI_PUBLIC(void) SachiInterpreter_NullObjectError(Sachi_Interpreter* InInterpreter)
{
	SachiInterpreter_SetErrorMessage(InInterpreter, "object is null");
}