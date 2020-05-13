#ifndef SACHI_NODE_H
#define SACHI_NODE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
	
typedef struct _Sachi_Pin Sachi_Pin;
typedef struct _Sachi_Node Sachi_Node;
typedef struct _Sachi_Instance Sachi_Instance;
typedef struct _Sachi_Link Sachi_Link;
typedef struct _Sachi_Flow Sachi_Flow;

#define SACHINODE_PINMODE_EXEC 0x1
#define SACHINODE_PINMODE_VALUE 0x2
#define SACHINODE_PINSIDE_IN 0x1
#define SACHINODE_PINSIDE_OUT 0x2

#define Sachi_PinMode unsigned char
#define Sachi_PinSide unsigned char

typedef struct _Sachi_Pin
{
	char* Label;
	char* Name;
	Sachi_PinMode Mode;
	Sachi_PinSide Side;
	char* Type;
	char* Default;
} Sachi_Pin;

typedef struct _Sachi_Node
{
	char* Name;
	Sachi_Node* Nodes;
	sachi_size_t NumNodes;
	Sachi_Pin* Pins;
	sachi_size_t NumPins;
	Sachi_Flow* Flow;
} Sachi_Node;

typedef struct _Sachi_Instance
{
	char* Path;
	char* Name;
} Sachi_Instance;

typedef struct _Sachi_Link
{
	char* From;
	char* To;
} Sachi_Link;

typedef struct _Sachi_Flow
{
	Sachi_Instance* Instances;
	sachi_size_t NumInstances;
	Sachi_Link* Links;
	sachi_size_t NumLinks;
} Sachi_Flow;

SACHI_PUBLIC(Sachi_Pin*) Sachi_NewPin();
SACHI_PUBLIC(Sachi_Pin*) Sachi_NewPinWithLength(sachi_size_t);
SACHI_PUBLIC(void) Sachi_InitPin(Sachi_Pin*);
SACHI_PUBLIC(void) Sachi_ResetPin(Sachi_Pin*);
SACHI_PUBLIC(void) Sachi_DeletePin(Sachi_Pin*);
SACHI_PUBLIC(void) Sachi_DeletePinWithLength(Sachi_Pin*, sachi_size_t);
SACHI_PUBLIC(int) Sachi_PinModeFromString(const char* InStr, Sachi_PinMode* OutValue);
SACHI_PUBLIC(int) Sachi_PinSideFromString(const char* InStr, Sachi_PinSide* OutValue);

SACHI_PUBLIC(Sachi_Node*) Sachi_NewNode();
SACHI_PUBLIC(Sachi_Node*) Sachi_NewNodeWithLength(sachi_size_t);
SACHI_PUBLIC(void) Sachi_InitNode(Sachi_Node*);
SACHI_PUBLIC(void) Sachi_ResetNode(Sachi_Node*);
SACHI_PUBLIC(void) Sachi_DeleteNode(Sachi_Node*);
SACHI_PUBLIC(void) SachNodei_DeleteNodeWithLength(Sachi_Node*, sachi_size_t);
#ifdef SACHI_LOG_ENABLED
SACHI_PUBLIC(void) Sachi_PrintNode(Sachi_Node* InNode);
#endif

#ifdef __cplusplus
}
#endif

#endif
