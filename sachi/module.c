#include "sachi/module.h"
#include "sachi/posix.h"

#define SACHI_NEWGENERIC(var, type, init) \
type* var = sachi_malloc(sizeof(type)); \
init(var)

#define SACHI_NEWGENERICWITHLENGTH(var, type, init, length) \
type* var = sachi_malloc(sizeof(type) * length); \
for (sachi_size_t I = 0; I < length; ++I) \
{ \
	init(&var[I]); \
}

#define SACHI_NEWGENERIC_RETURN(type, init) \
SACHI_NEWGENERIC(O, type, init); \
return O

#define SACHI_NEWGENERICWITHLENGTH_RETURN(type, init, length) \
SACHI_NEWGENERICWITHLENGTH(O, type, init, length); \
return O

#define SACHI_GENERATENEW(name) \
SACHI_PUBLIC(Sachi_##name *) Sachi_New##name() \
{ \
	SACHI_NEWGENERIC_RETURN(Sachi_##name, Sachi_Init##name); \
} \
 \
SACHI_PUBLIC(Sachi_##name *) Sachi_New##name##WithLength(sachi_size_t InLength) \
{ \
	SACHI_NEWGENERICWITHLENGTH_RETURN(Sachi_##name, Sachi_Init##name, InLength); \
}

#define SACHI_GENERATEDELETE(name) \
SACHI_PUBLIC(void) Sachi_Delete##name(Sachi_##name * InObject) \
{ \
	if (InObject) \
	{ \
		Sachi_Reset##name; \
		sachi_free(InObject); \
	} \
} \
 \
SACHI_PUBLIC(void) Sachi_Delete##name##WithLength(Sachi_##name * InObject, sachi_size_t InLength) \
{ \
	if (InObject) \
	{ \
		for (sachi_size_t I = 0; I < InLength; ++I) \
		{ \
			Sachi_Reset##name(&InObject[I]); \
		} \
		sachi_free(InObject); \
	} \
}

SACHI_GENERATENEW(NodePin);
SACHI_GENERATEDELETE(NodePin);
SACHI_GENERATENEW(Node);
SACHI_GENERATEDELETE(Node);

SACHI_PUBLIC(void) Sachi_InitNodePin(Sachi_NodePin* InObject)
{
	InObject->Name = NULL;
}

SACHI_PUBLIC(void) Sachi_ResetNodePin(Sachi_NodePin* InObject)
{
	if (InObject->Name != NULL)
	{
		sachi_free(InObject->Name);
		InObject->Name = NULL;
	}
}

SACHI_PUBLIC(void) Sachi_InitNode(Sachi_Node* InObject)
{
	InObject->Name = NULL;
	InObject->Inputs = NULL;
	InObject->NumInputs = 0;
	InObject->Outputs = NULL;
	InObject->NumOutputs = 0;
	InObject->Nodes = NULL;
	InObject->NumNodes = 0;
}

SACHI_PUBLIC(void) Sachi_ResetNode(Sachi_Node* InObject)
{
	if (InObject->Name != NULL)
	{
		sachi_free(InObject->Name);
		InObject->Name = NULL;
	}
	InObject->Inputs = NULL;
	InObject->NumInputs = 0;
	InObject->Outputs = NULL;
	InObject->NumOutputs = 0;
	InObject->Nodes = NULL;
	InObject->NumNodes = 0;
}