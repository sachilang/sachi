#ifndef SACHI_OBJECT_INTERPRETER_H
#define SACHI_OBJECT_INTERPRETER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

extern Sachi_ObjectType Sachi_InterpreterType;

typedef struct _Sachi_NodeMetadata Sachi_NodeMetadata;
typedef struct _Sachi_Interpreter Sachi_Interpreter;

#define Sachi_CheckInterpreter(o) (o->Type == &Sachi_InterpreterType)

/**
 * Create a new instance of interpreter.
 *
 * It will have an initial stack frame.
 *
 * :param InInterpreter: parent interpreter instance
 * :return: interpreter instance or NULL
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewInterpreter(Sachi_Interpreter* InInterpreter);

/**
 * Delete an interpreter instance.
 *
 * Free up memory.
 *
 * :param InInterpreter: interpreter instance
 */
SACHI_PUBLIC(void) Sachi_DeleteInterpreter(Sachi_Object* InObject);

/**
 * Get the current error message.
 *
 * :param InInterpreter: interpreter instance
 * :return: error message or NULL
 */
SACHI_PUBLIC(const char*) SachiInterpreter_GetErrorMessage(Sachi_Object* InObject);

/**
 * Set the error message.
 *
 * :param InInterpreter: interpreter instance
 * :param InBuffer: New message
 */
SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessage(Sachi_Object* InObject, const char* InBuffer);

/**
 * Set the error message with maximum length.
 *
 * :param InInterpreter: interpreter instance
 * :param InBuffer: New message
 * :param InBufferLength: Message length
 */
SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessageWithLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InBufferLength);

/**
 * Called when a memory allocation failed.
 *
 * :param InInterpreter: interpreter instance
 */
SACHI_PUBLIC(void) SachiInterpreter_MemoryAllocationError(Sachi_Object* InObject);

/**
 * Add a node to the interpreter.
 *
 * This makes the node accessible from code.
 *
 * :param InObject: interpreter instance
 * :param InNode: node to add
 * :return: error code
 */
SACHI_PUBLIC(int) SachiInterpreter_AddNode(Sachi_Object* InObject, Sachi_Object* InNode);
SACHI_PUBLIC(int) SachiInterpreter_AddNodeFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata);
SACHI_PUBLIC(int) SachiInterpreter_AddNodeFromDict(Sachi_Object* InObject, Sachi_Object* InDict);

SACHI_PUBLIC(Sachi_Object*) Sachi_True(Sachi_Object* InInterpreter);
SACHI_PUBLIC(Sachi_Object*) Sachi_False(Sachi_Object* InInterpreter);
#define Sachi_IsTrue(interpreter, o) (((Sachi_Object*)o) == Sachi_True(interpreter))
#define Sachi_IsFalse(interpreter, o) (((Sachi_Object*)o) == Sachi_False(interpreter))

#ifdef __cplusplus
}
#endif

#endif