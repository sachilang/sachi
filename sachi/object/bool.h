#ifndef SACHI_BOOL_H
#define SACHI_BOOL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Bool
{
	Sachi_Object Base;
} Sachi_Bool;
	
extern Sachi_Bool _Sachi_False;
extern Sachi_Bool _Sachi_True;
extern Sachi_ObjectType Sachi_BoolType;
	
#define Sachi_False ((Sachi_Object*)&_Sachi_False)
#define Sachi_True ((Sachi_Object*)&_Sachi_True)
#define Sachi_IsFalse(o) (((Sachi_Object*)o) == Sachi_False)
#define Sachi_IsTrue(o) (((Sachi_Object*)o) == Sachi_True)

SACHI_PUBLIC(Sachi_Object*) Sachi_NewBool(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) Sachi_IsBool(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif