#ifndef SACHI_NODE_BUILTIN_H
#define SACHI_NODE_BUILTIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"
#include "sachi/nodemetadata.h"
    
extern Sachi_NodeMetadata Sachi_IfNodeMetadata;
extern Sachi_NodeMetadata Sachi_ForInNodeMetadata;
extern Sachi_NodeMetadata Sachi_WhileNodeMetadata;
extern Sachi_NodeMetadata Sachi_ImportNodeMetadata;
extern Sachi_NodeMetadata Sachi_PrintNodeMetadata;

/**
 * List all builtin nodes.
 */
extern Sachi_NodeMetadata* Sachi_BuiltinNodesMetadata[];

#ifdef __cplusplus
}
#endif

#endif