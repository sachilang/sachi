#include "sachi/object/bool.h"
#include "sachi/sachi.h"

Sachi_ObjectType Sachi_BoolType = {
	0 // I
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