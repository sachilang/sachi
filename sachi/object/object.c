#include "sachi/object/object.h"

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

SACHI_PUBLIC(int) Sachi_InitializeType(Sachi_ObjectType* InType)
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