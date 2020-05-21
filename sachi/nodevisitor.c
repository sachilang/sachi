#include "sachi/nodevisitor.h"
#include "sachi/object/node.h"
#include "sachi/object/list.h"

SACHI_PUBLIC(void) Sachi_VisitNode(Sachi_Object* InNode, Sachi_NodeVisitor* InVisitor)
{
	InVisitor->VisitNode(InNode);
	Sachi_Object* Pins = SachiNode_GetPins(InNode);
	SACHILIST_FOREACH(Pins, Index, Items)
	{
		InVisitor->VisitPin(*Items);
	}
	Sachi_Object* Flow = SachiNode_GetFlow(InNode);
	InVisitor->VisitFlow(Flow);
	SACHILIST_FOREACH(Flow, Index, Items)
	{
		InVisitor->VisitNodeInstance(*Items);
	}
}