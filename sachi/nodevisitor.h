#ifndef SACHI_NODEVISITOR_H
#define SACHI_NODEVISITOR_H

#ifdef __cplusplus
extern "C"
{
#endif
     
#include "sachi/config.h"
#include "sachi/posix.h"

typedef struct _Sachi_Object Sachi_Object;

/**
 * Metadata of a node.
 *
 * This is how a node is described internally and externally.
 */
typedef struct _Sachi_NodeVisitor
{
    void(*VisitNode)(Sachi_Object*);
    void(*VisitPin)(Sachi_Object*);
    void(*VisitFlow)(Sachi_Object*);
    void(*VisitNodeInstance)(Sachi_Object*);
} Sachi_NodeVisitor;

SACHI_PUBLIC(void) Sachi_VisitNode(Sachi_Object* InNode, Sachi_NodeVisitor* InVisitor);

#ifdef __cplusplus
}
#endif

#endif