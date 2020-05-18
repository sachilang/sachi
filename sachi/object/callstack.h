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

typedef struct _Sachi_CallStack Sachi_CallStack;

#define Sachi_CheckCallStack(o) (o->Type == &Sachi_CallStackType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStack(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStackFromBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewCallStackFromBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength);
SACHI_PUBLIC(void) Sachi_DeleteCallStack(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiCallStack_Empty(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiCallStack_Size(Sachi_Object* InObject);

/**
 * Get the underlying allocated memory array.
 *
 * :param InObject: list
 * :return: underlying memory array
 */
SACHI_PUBLIC(const char*) SachiCallStack_Data(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif