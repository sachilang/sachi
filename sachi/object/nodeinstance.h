#ifndef SACHI_OBJECT_NODEINSTANCE_H
#define SACHI_OBJECT_NODEINSTANCE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;

/**
 * Representation of a call to node in memory.
 */
extern Sachi_ObjectType Sachi_NodeInstanceType;

/**
 * Create a new node instance from existing node.
 *
 * :param InInterpreter: interpreter instance
 * :param InNode: node
 * :return: new instance
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeInstance(Sachi_Interpreter* InInterpreter, Sachi_Object* InNode);

/**
 * Delete a node instance.
 *
 * :param InObject: node instance to delete
 */
SACHI_PUBLIC(void) Sachi_DeleteNodeInstance(Sachi_Object* InObject);

/**
 * External access to InputExecPin is write and read.
 */
SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetInputExecPin(const Sachi_Object* InObject);
SACHI_PUBLIC(void) SachiNodeInstance_SetInputExecPin(Sachi_Object* InObject, Sachi_Object* InPin);

/**
 * External access to KwArgs is write and read.
 */
SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetKwArgs(const Sachi_Object* InObject);
SACHI_PUBLIC(void) SachiNodeInstance_SetKwArgs(Sachi_Object* InObject, Sachi_Object* InKwArgs);

/**
 * External access to OutputExecPin is read only.
 */
SACHI_PUBLIC(Sachi_Object*) SachiNodeInstance_GetOutputExecPin(const Sachi_Object* InObject);

/**
 * External access to KwResults is read only.
 */
SACHI_PUBLIC(const Sachi_Object*) SachiNodeInstance_GetKwResults(const Sachi_Object* InObject);

/**
 * Call this node.
 *
 * It must have been parametrized via getters/setters beforehand.
 */
SACHI_PUBLIC(int) SachiNodeInstance_Call(Sachi_Object* InObject, Sachi_Object* InCallStack);

/**
 * Call this node.
 */
SACHI_PUBLIC(int) SachiNodeInstance_CallWithArgs(Sachi_Object* InObject, Sachi_Object* InCallStack, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object** OutKwResults);

#ifdef __cplusplus
}
#endif

#endif