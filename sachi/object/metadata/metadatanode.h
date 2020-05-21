#ifndef SACHI_OBJECT_METADATA_METADATANODE_H
#define SACHI_OBJECT_METADATA_METADATANODE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;
typedef struct _Sachi_PinMetadata Sachi_PinMetadata;
typedef struct _Sachi_NodeMetadata Sachi_NodeMetadata;

/**
 * Create a new node.
 *
 * Node's metadata are initialized from external metadata.
 *
 * :param InInterpreter: interpreter instance
 * :param InMetadata: metadata
 * :return: new node
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewNodeFromMetadata(Sachi_Interpreter* InInterpreter, Sachi_NodeMetadata* InMetadata);

/**
 * Set the metadata of this node.
 *
 * :param InObject: instance
 * :param InMetadata: metadata
 * :return: error code
 */
SACHI_PUBLIC(int) SachiNode_IntFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InMetadata);
SACHI_PUBLIC(int) SachiNode_SetPinsFromMetadata(Sachi_Object* InObject, Sachi_PinMetadata* InPins);
SACHI_PUBLIC(int) SachiNode_SetNodesFromMetadata(Sachi_Object* InObject, Sachi_NodeMetadata* InNodes);

#ifdef __cplusplus
}
#endif

#endif