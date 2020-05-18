#include "sachi/object/bool.h"
#include "sachi/object/interpreter.h"
#include "sachi/sachi.h"

typedef struct _Sachi_Bool
{
	SACHI_OBJECT_HEADER
} Sachi_Bool;

Sachi_ObjectType Sachi_BoolType = {
	"bool",
	sizeof(Sachi_Bool),
	NULL, // base
	NULL, // new
	NULL, // delete
	NULL, // nodes
	NULL, // hash
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewBool(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewObject(InInterpreter, &Sachi_BoolType);
}

SACHI_PUBLIC(void) Sachi_DeleteBool(Sachi_Object* InObject)
{
	Sachi_DeleteObject(InObject);
}