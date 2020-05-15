#include "sachi/object/bool.h"
#include "sachi/sachi.h"
#include "sachi/interpreter.h"

static Sachi_Object* _Sachi_NewBool(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewBool(InInterpreter);
}

static void _Sachi_DeleteBool(Sachi_Object* InObject)
{
	Sachi_DeleteBool(InObject);
}

Sachi_ObjectType Sachi_BoolType = {
	"bool",
	NULL, // base
	NULL, // new
	NULL, // delete
	NULL, // nodes
	NULL, // hash
};

Sachi_Bool _Sachi_False = {
	{
		&Sachi_BoolType
	}
};

Sachi_Bool _Sachi_True = {
	{
		&Sachi_BoolType
	}
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewBool(Sachi_Object* InObject)
{
	if (InObject)
	{
		return Sachi_True;
	}

	return Sachi_False;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_IsBool(Sachi_Object* InObject)
{
	if (InObject->Type == &Sachi_BoolType)
	{
		return Sachi_True;
	}

	return Sachi_False;
}