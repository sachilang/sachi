#ifndef SACHI_OBJECT_JSON_JSONNODE_H
#define SACHI_OBJECT_JSON_JSONNODE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;

/**
 * Create a new node with an empty definition.
 *
 * Node's metadata are initialized from dict.
 *
 * :param InInterpreter: interpreter instance
 * :param InDict: dict containing metadata
 * :return: new node
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromJSON(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict);

/**
 * Set the metadata of this node.
 *
 * :param InObject: instance
 * :param InDict: dict containing metadata
 * :return: error code
 */
SACHI_PUBLIC(int) SachiNode_InitFromJSON(Sachi_Object* InObject, Sachi_Object* InDict);

#ifdef __cplusplus
}
#endif

#endif