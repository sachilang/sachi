#ifndef SACHI_FLOW_H
#define SACHI_FLOW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"

#define SACHI_MODULEIMPORT_LIB 0

typedef struct _Sachi_ModuleImport
{
	char Type;
} Sachi_ModuleImport;

struct _Sachi_Link;
typedef struct _Sachi_Link
{
	int I;
} Sachi_Link;

struct _Sachi_NodePin;
typedef struct _Sachi_NodePin
{
	char* Name;
} Sachi_NodePin;

SACHI_PUBLIC(Sachi_NodePin*) Sachi_NewNodePin();
SACHI_PUBLIC(Sachi_NodePin*) Sachi_NewNodePinWithLength(sachi_size_t);
SACHI_PUBLIC(void) Sachi_InitNodePin(Sachi_NodePin*);
SACHI_PUBLIC(void) Sachi_ResetNodePin(Sachi_NodePin*);
SACHI_PUBLIC(void) Sachi_DeleteNodePin(Sachi_NodePin*);
SACHI_PUBLIC(void) Sachi_DeleteNodePinWithLength(Sachi_NodePin*, sachi_size_t);

struct _Sachi_Node;
typedef struct _Sachi_Node
{
	char* Name;
	Sachi_ModuleImport* Imports;
	sachi_size_t NumImports;
	struct _Sachi_NodePin* Inputs;
	sachi_size_t NumInputs;
	struct _Sachi_NodePin* Outputs;
	sachi_size_t NumOutputs;
	struct _Sachi_Node* Defines;
	sachi_size_t NumDefines;
	struct _Sachi_Node* Nodes;
	sachi_size_t NumNodes;
	struct _Sachi_Link* Links;
	sachi_size_t NumLinks;
} Sachi_Node;

SACHI_PUBLIC(Sachi_Node*) Sachi_NewNode();
SACHI_PUBLIC(Sachi_Node*) Sachi_NewNodeWithLength(sachi_size_t);
SACHI_PUBLIC(void) Sachi_InitNode(Sachi_Node*);
SACHI_PUBLIC(void) Sachi_ResetNode(Sachi_Node*);
SACHI_PUBLIC(void) Sachi_DeleteNode(Sachi_Node*);
SACHI_PUBLIC(void) Sachi_DeleteNodeWithLength(Sachi_Node*, sachi_size_t);

#ifdef __cplusplus
}
#endif

#endif