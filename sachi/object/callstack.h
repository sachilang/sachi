#ifndef SACHI_OBJECT_CALLSTACK_H
#define SACHI_OBJECT_CALLSTACK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_CallStackType;

typedef struct _Sachi_NodeMetadata Sachi_NodeMetadata;
typedef struct _Sachi_CallStack Sachi_CallStack;

#define Sachi_CheckCallStack(o) (o->Type == &Sachi_CallStackType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStack(Sachi_Interpreter* InInterpreter, Sachi_Object* InParent);
SACHI_PUBLIC(void) Sachi_DeleteCallStack(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiCallStack_AddNode(Sachi_Object* InObject, Sachi_Object* InNode);
SACHI_PUBLIC(int) SachiCallStack_AddNodeFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata);
SACHI_PUBLIC(int) SachiCallStack_AddNodeFromDict(Sachi_Object* InObject, Sachi_Object* InDict);

#ifdef __cplusplus
}
#endif

#endif