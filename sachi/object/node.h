#ifndef SACHI_OBJECT_NODE_H
#define SACHI_OBJECT_NODE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"
#include "sachi/nodemetadata.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;

extern Sachi_ObjectType Sachi_NodeType;

#define Sachi_CheckNode(o) (o->Type == &Sachi_NodeType)

/**
 * Create a new node with empty metadata.
 *
 * :param InInterpreter: interpreter instance
 * :return: new node
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewNode(Sachi_Interpreter* InInterpreter);

/**
 * Create a new node with an empty definition.
 *
 * Node's metadata are initialized from dict.
 *
 * :param InInterpreter: interpreter instance
 * :param InDict: dict containing metadata
 * :return: new node
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromDict(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict);

/**
 * Delete a node.
 *
 * :param InObject: node to delete
 */
SACHI_PUBLIC(void) Sachi_DeleteNode(Sachi_Object* InObject);

/**
 * Set the metadata of this node.
 *
 * :param InObject: instance
 * :param InDict: dict containing metadata
 * :return: error code
 */
SACHI_PUBLIC(int) SachiNode_InitFromDict(Sachi_Object* InObject, Sachi_Object* InDict);

SACHI_PUBLIC(const char*) SachiNode_GetName(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiNode_SetName(Sachi_Object* InObject, Sachi_Object* InName);
SACHI_PUBLIC(int) SachiNode_SetNameFromBuffer(Sachi_Object* InObject, const char* InBuffer);
SACHI_PUBLIC(int) SachiNode_SetNameFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength);
SACHI_PUBLIC(Sachi_CFunc) SachiNode_GetFunc(Sachi_Object* InObject);
SACHI_PUBLIC(void) SachiNode_SetFunc(Sachi_Object* InObject, Sachi_CFunc InFunc);
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPins(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiNode_SetPins(Sachi_Object* InObject, Sachi_Object* InPins);
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPin(Sachi_Object* InObject, const char* InName);
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetNodes(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiNode_SetNodes(Sachi_Object* InObject, Sachi_Object* InNodes);
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetNode(Sachi_Object* InObject, const char* InName);
SACHI_PUBLIC(void) SachiNode_SetFlow(Sachi_Object* InObject, Sachi_Object* InFlow);
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetFlow(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif