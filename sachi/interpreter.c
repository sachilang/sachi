#include "sachi/interpreter.h"
#include "sachi/posix.h"
#include "sachi/node.h"

typedef struct _Sachi_InterpreterStack Sachi_InterpreterStack;
typedef struct _Sachi_InterpreterStack
{
	Sachi_InterpreterStack* Parent;
	Sachi_Node* Node;
} Sachi_InterpreterStack;

typedef struct _Sachi_Interpreter
{
	Sachi_InterpreterStack* Stack;
	sachi_size_t StackDepth;
} Sachi_Interpreter;

typedef struct {
	const char* message;
	size_t position;
} error;
static error global_error = { NULL, 0 };

SACHI_PUBLIC(const char*) SachiInterpreter_GetErrorPtr(void)
{
	return (const char*)(global_error.message + global_error.position);
}

static void SachiInterpreter_SetErrorMemoryAllocation()
{
	global_error.message = "memory allocation failed";
}

static void SachiInterpreter_SetErrorNULL()
{
	global_error.message = "invalid NULL pointer";
}

SACHI_PUBLIC(Sachi_Interpreter*) SachiInterpreter_NewInterpreter()
{
	Sachi_Interpreter* O = sachi_malloc(sizeof(Sachi_Interpreter));
	if (O)
	{
		O->Stack = NULL;
		O->StackDepth = 0;
	}

	return O;
}

static int SachiInterpeter_IncreaseStack(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode)
{
	Sachi_InterpreterStack* Current = (Sachi_InterpreterStack*)sachi_malloc(sizeof(Sachi_InterpreterStack));
	if (!Current)
	{
		SachiInterpreter_SetErrorMemoryAllocation();
		return SACHI_ERROR;
	}

	Current->Parent = InInterpreter->Stack;
	Current->Node = InNode;
	InInterpreter->Stack = Current;
	InInterpreter->StackDepth++;
#ifdef SACHI_LOG_ENABLED
	sachi_printf("interpreter stack increased for node %s\n", InNode->Name);
#endif

	return SACHI_OK;
}

static int SachiInterpeter_DecreaseStack(Sachi_Interpreter* InInterpreter)
{
	Sachi_InterpreterStack* Current = InInterpreter->Stack;
	if (Current)
	{
		InInterpreter->Stack = Current->Parent;
		InInterpreter->StackDepth--;
		sachi_free(Current);
	}

#ifdef SACHI_LOG_ENABLED
	sachi_printf("interpreter stack decreased\n");
#endif

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiInterpreter_RunNode(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode)
{
	if (SachiInterpeter_IncreaseStack(InInterpreter, InNode) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	// Start from output pin

	if (SachiInterpeter_DecreaseStack(InInterpreter) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) SachiInterpreter_RunMain(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode)
{
	return SachiInterpreter_RunNode(InInterpreter, InNode);
}