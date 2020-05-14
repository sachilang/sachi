#include "sachi/interpreter.h"
#include "sachi/sachi.h"
#include "sachi/posix.h"
#include "sachi/node.h"

typedef struct _Sachi_NodeInstance
{
	Sachi_Node* Node;
	char* Name;
} Sachi_NodeInstance;

typedef struct _Sachi_InterpreterStack Sachi_InterpreterStack;
typedef struct _Sachi_InterpreterStack
{
	Sachi_InterpreterStack* Parent;
	// Nodes loaded in this stack frame
	Sachi_Node** Nodes;
	sachi_size_t NumNodes;
	// Instances of nodes created in this stack frame
	Sachi_NodeInstance* Instances;
	sachi_size_t NumInstances;
	// Executed flow
	Sachi_Flow* Flow;
	Sachi_Link* Link;
} Sachi_InterpreterStack;

typedef struct _Sachi_Interpreter
{
	Sachi_InterpreterStack* Stack;
	sachi_size_t StackDepth;
	struct {
		const char* Message;
	} Error;
} Sachi_Interpreter;

static void SachiInterpreter_SetErrorMemoryAllocation()
{
	Sachi_SetErrorPtr("memory allocation failed");
}

static void SachiInterpreter_SetErrorNULL()
{
	Sachi_SetErrorPtr("invalid NULL pointer");
}

//********
// PRIVATE
//********
SACHI_PUBLIC(void) SachiInterpreter_ResetStack(Sachi_InterpreterStack* InStack)
{
	if (!InStack)
	{
		return;
	}

	InStack->Parent = NULL;
	sachi_free(InStack->Nodes);
	InStack->Nodes = NULL;
	InStack->NumNodes = 0;
	InStack->Flow = NULL;
	InStack->Link = NULL;
	// Todo instances
}

SACHI_PUBLIC(void) SachiInterpreter_DeleteStack(Sachi_InterpreterStack* InStack)
{
	SachiInterpreter_ResetStack(InStack);
	sachi_free(InStack);
}

static int SachiInterpreter_InitStack(Sachi_InterpreterStack* InStack)
{
	InStack->Parent = NULL;
	InStack->Nodes = NULL;
	InStack->NumNodes = 0;
	InStack->Instances = NULL;
	InStack->NumInstances = 0;
	InStack->Flow = NULL;
	InStack->Link = NULL;

	return SACHI_OK;
}

static Sachi_InterpreterStack* SachiInterpreter_NewStack()
{
	Sachi_InterpreterStack* Stack = (Sachi_InterpreterStack*)sachi_malloc(sizeof(Sachi_InterpreterStack));
	if (!Stack)
	{
		SachiInterpreter_SetErrorMemoryAllocation();
		return NULL;
	}

	if (SachiInterpreter_InitStack(Stack) != SACHI_OK)
	{
		SachiInterpreter_DeleteStack(Stack);
		return SACHI_ERROR;
	}

	return Stack;
}

//********
// PUBLIC
//********
SACHI_PUBLIC(Sachi_Interpreter*) Sachi_NewInterpreter()
{
	Sachi_Interpreter* Interpreter = sachi_malloc(sizeof(Sachi_Interpreter));
	if (!Interpreter)
	{
		SachiInterpreter_SetErrorMemoryAllocation();
		return NULL;
	}

	if (Sachi_InitInterpreter(Interpreter) != SACHI_OK)
	{
		Sachi_DeleteInterpreter(Interpreter);
		return NULL;
	}

	return Interpreter;
}

SACHI_PUBLIC(int) Sachi_InitInterpreter(Sachi_Interpreter* InInterpreter)
{
	InInterpreter->Stack = NULL;
	InInterpreter->StackDepth = 0;
	InInterpreter->Error.Message = NULL;

	if (SachiInterpeter_IncreaseStack(InInterpreter) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(void) Sachi_ResetInterpreter(Sachi_Interpreter* InInterpreter)
{
	if (!InInterpreter)
	{
		return;
	}

	while (InInterpreter->Stack)
	{
		Sachi_InterpreterStack* Parent = InInterpreter->Stack->Parent;
		SachiInterpreter_DeleteStack(InInterpreter->Stack);
		InInterpreter->Stack = Parent;
	}
	InInterpreter->Stack = NULL;
	InInterpreter->StackDepth = 0;
}

SACHI_PUBLIC(void) Sachi_DeleteInterpreter(Sachi_Interpreter* InInterpreter)
{
	Sachi_ResetInterpreter(InInterpreter);
	sachi_free(InInterpreter);
}

SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessage(Sachi_Interpreter* InInterpreter, const char* InBuffer)
{
	InInterpreter->Error.Message = InBuffer;
}

SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessageWithLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InBufferLength)
{
	InInterpreter->Error.Message = InBuffer;
}

SACHI_PUBLIC(void) SachiInterpreter_MemoryAllocationError(Sachi_Interpreter* InInterpreter)
{
	SachiInterpreter_SetErrorMessage(InInterpreter, "memory allocation failed");
}

SACHI_PUBLIC(int) SachiInterpeter_IncreaseStack(Sachi_Interpreter* InInterpreter)
{
	Sachi_InterpreterStack* Current = SachiInterpreter_NewStack();
	if (!Current)
	{
		return SACHI_ERROR;
	}

	Current->Parent = InInterpreter->Stack;
	InInterpreter->Stack = Current;
	InInterpreter->StackDepth++;
	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiInterpeter_DecreaseStack(Sachi_Interpreter* InInterpreter)
{
	Sachi_InterpreterStack* Current = InInterpreter->Stack;
	if (Current)
	{
		InInterpreter->Stack = Current->Parent;
		InInterpreter->StackDepth--;
		SachiInterpreter_DeleteStack(Current);
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiInterpreter_LoadNode(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode)
{
	Sachi_InterpreterStack* Stack = InInterpreter->Stack;
	Sachi_Node** NewNodes = (Sachi_Node**)sachi_realloc(Stack->Nodes, sizeof(Sachi_Node*) * (Stack->NumNodes + 1));
	if (!NewNodes)
	{
		SachiInterpreter_SetErrorMemoryAllocation();
		return SACHI_ERROR;
	}

	NewNodes[Stack->NumNodes] = InNode;
	Stack->Nodes = NewNodes;
	Stack->NumNodes++;

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiInterpreter_UnloadNode(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode)
{
	return SACHI_OK;
}

static Sachi_Node* SachiInterpreter_FindNodeInStack(Sachi_InterpreterStack* InStack, const char* InPath)
{
	Sachi_Node** Node = InStack->Nodes;
	for (sachi_size_t I = 0; I < InStack->NumNodes; ++I)
	{
		if (sachi_strcmp((*Node)->Name, InPath) == 0)
		{
			return *Node;
		}
		Node++;
	}

	return NULL;
}

SACHI_PUBLIC(Sachi_Node*) SachiInterpreter_FindNode(Sachi_Interpreter* InInterpreter, const char* InPath)
{
	Sachi_Node* Node = NULL;
	Sachi_InterpreterStack* Stack = InInterpreter->Stack;
	while (Stack)
	{
		Node = SachiInterpreter_FindNodeInStack(Stack, InPath);
		if (Node)
		{
			return Node;
		}
		Stack = Stack->Parent;
	}

	return NULL;
}

SACHI_PUBLIC(int) SachiInterpreter_InstantiateNode(Sachi_Interpreter* InInterpreter, const char* InPath, const char* InName)
{
	Sachi_Node* Node = SachiInterpreter_FindNode(InInterpreter, InPath);
	if (!Node)
	{
		Sachi_SetErrorPtr("node not found");
		return SACHI_ERROR;
	}



	return SACHI_OK;
}

/**
 * Given a node, find the input and output exec pins.
 *
 * :param InNode: node
 * :param OutInputExecPin: input exec pin
 * :param OutOutputExecPin: output exec pin
 * :return: error code
 */
static int SachiInterpreter_GetDefaultExecPins(Sachi_Node* InNode, Sachi_Pin** OutInputExecPin, Sachi_Pin** OutOutputExecPin)
{
	*OutInputExecPin = NULL;
	*OutOutputExecPin = NULL;
	Sachi_Pin* Pin = InNode->Pins;
	for (sachi_size_t I = 0; I < InNode->NumPins; ++I)
	{
		if (Pin->Mode == SACHINODE_PINMODE_EXEC)
		{
			if (Pin->Side == SACHINODE_PINSIDE_IN)
			{
				*OutInputExecPin = Pin;
			}
			else
			{
				*OutOutputExecPin = Pin;
			}
		}
		Pin++;
	}

	if (!*OutInputExecPin || !*OutOutputExecPin)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

static Sachi_Instance* SachiInterpreter_FindInstance(Sachi_Interpreter* InInterpreter, const char* InName)
{
	Sachi_Flow* Flow = InInterpreter->Stack->Flow;

	Sachi_Instance* Instance = Flow->Instances;
	for (sachi_size_t I = 0; I < Flow->NumInstances; ++I)
	{
		if (sachi_strcmp(Instance->Name, InName) == 0)
		{
			return Instance;
		}
		Instance++;
	}

	return NULL;
}

static int SachiInterpreter_Step(Sachi_Interpreter* InInterpreter)
{
	Sachi_InterpreterStack* Stack = InInterpreter->Stack;
	Sachi_Flow* Flow = Stack->Flow;
	Sachi_Link* Link = Stack->Link;

	Sachi_Instance* Instance = SachiInterpreter_FindInstance(InInterpreter, Link->To.InstanceName);
	if (!Instance)
	{
		Sachi_SetErrorPtr("instance not found");
		return SACHI_ERROR;
	}

	Sachi_Node* Node = SachiInterpreter_FindNode(InInterpreter, Instance->Path);
	if (!Node)
	{
		Sachi_SetErrorPtr("node not found");
		return SACHI_ERROR;
	}

	sachi_printf("executing node %s", Node->Name);
	Sachi_RunFlow(InInterpreter, Node->Flow);

	return SACHI_OK;
}

SACHI_PUBLIC(int) Sachi_RunFlow(Sachi_Interpreter* InInterpreter, Sachi_Flow* InFlow)
{
	if (SachiInterpeter_IncreaseStack(InInterpreter) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	// Search the this.execin node
	Sachi_Link* ExecLink = NULL;
	Sachi_Link* Link = InFlow->Links;
	for (sachi_size_t I = 0; I < InFlow->NumLinks; ++I)
	{
		if (sachi_strcmp(Link->From.InstanceName, "this") == 0 && sachi_strcmp(Link->From.PinName, "execin") == 0)
		{
			ExecLink = Link;
			break;
		}
		Link++;
	}

	if (!ExecLink)
	{
		Sachi_SetErrorPtr("this.execin not linked");
		goto fail;
	}

	InInterpreter->Stack->Flow = InFlow;
	InInterpreter->Stack->Link = ExecLink;
	SachiInterpreter_Step(InInterpreter);

	SachiInterpeter_DecreaseStack(InInterpreter);
	
	return SACHI_OK;

fail:
	SachiInterpeter_DecreaseStack(InInterpreter);
	return SACHI_ERROR;
}