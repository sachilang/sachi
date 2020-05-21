#ifndef SACHI_OBJECT_STRING_H
#define SACHI_OBJECT_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_StringType;

typedef struct _Sachi_String Sachi_String;

#define Sachi_IsString(o) (o->Type == &Sachi_StringType)
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewString(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewStringFromBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer);
SACHI_PUBLIC(Sachi_Object*) Sachi_NewStringFromBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength);
SACHI_PUBLIC(void) Sachi_DeleteString(Sachi_Object* InObject);
SACHI_PUBLIC(Sachi_Object*) SachiString_Empty(Sachi_Object* InObject);
SACHI_PUBLIC(sachi_size_t) SachiString_Size(Sachi_Object* InObject);
SACHI_PUBLIC(LONG) SachiString_Hash(Sachi_Object* InObject);
SACHI_PUBLIC(const char*) SachiString_ToString(Sachi_Object* InObject);

/**
 * Get the underlying allocated memory array.
 *
 * :param InObject: list
 * :return: underlying memory array
 */
SACHI_PUBLIC(const char*) SachiString_Data(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif