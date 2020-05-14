#ifndef SACHI_INTERPRETER_H
#define SACHI_INTERPRETER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
	
typedef struct _Sachi_Node Sachi_Node;
typedef struct _Sachi_Pin Sachi_Pin;
typedef struct _Sachi_Flow Sachi_Flow;
typedef struct _Sachi_Interpreter Sachi_Interpreter;

/**
 * Create a new instance of interpreter.
 *
 * It will have an initial stack frame.
 *
 * :return: interpreter instance or NULL
 */
SACHI_PUBLIC(Sachi_Interpreter*) Sachi_NewInterpreter();

/**
 * Initialize an interpreter instance.
 *
 * It will have an initial stack frame.
 *
 * :param InInterpreter: interpreter instance
 * :return: error code
 */
SACHI_PUBLIC(int) Sachi_InitInterpreter(Sachi_Interpreter* InInterpreter);

/**
 * Reset an interpreter instance.
 *
 * Don't free up memory.
 *
 * :param InInterpreter: interpreter instance
 */
SACHI_PUBLIC(void) Sachi_ResetInterpreter(Sachi_Interpreter* InInterpreter);

/**
 * Delete an interpreter instance.
 *
 * Free up memory.
 *
 * :param InInterpreter: interpreter instance
 */
SACHI_PUBLIC(void) Sachi_DeleteInterpreter(Sachi_Interpreter* InInterpreter);

/**
 * Get the current error message.
 *
 * :param InInterpreter: interpreter instance
 * :return: error message or NULL
 */
SACHI_PUBLIC(const char*) SachiInterpreter_GetErrorMessage(Sachi_Interpreter* InInterpreter);

/**
 * Set the error message.
 *
 * :param InInterpreter: interpreter instance
 * :param InBuffer: New message
 */
SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessage(Sachi_Interpreter* InInterpreter, const char* InBuffer);

/**
 * Set the error message with maximum length.
 *
 * :param InInterpreter: interpreter instance
 * :param InBuffer: New message
 * :param InBufferLength: Message length
 */
SACHI_PUBLIC(void) SachiInterpreter_SetErrorMessageWithLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InBufferLength);

/**
 * Called when a memory allocation failed.
 *
 * :param InInterpreter: interpreter instance
 */
SACHI_PUBLIC(void) SachiInterpreter_MemoryAllocationError(Sachi_Interpreter* InInterpreter);

/**
 * Push a new frame on top of interpreter's stack.
 *
 * :param InInterpreter: interpreter instance
 * :return: error code
 */
SACHI_PUBLIC(int) SachiInterpeter_IncreaseStack(Sachi_Interpreter* InInterpreter);

/**
 * Pop a frame from the top of interpreter's stack.
 *
 * :param InInterpreter: interpreter instance
 * :return: error code
 */
SACHI_PUBLIC(int) SachiInterpeter_DecreaseStack(Sachi_Interpreter* InInterpreter);

/**
 * Load a new node into the interpreter.
 *
 * It will be accessible from flows.
 *
 * :param InInterpreter: interpreter instance
 * :param InNode: node instance
 * :return: error code
 */
SACHI_PUBLIC(int) SachiInterpreter_LoadNode(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode);

/**
 * Load a new node into the interpreter.
 *
 * It will be accessible from flows.
 *
 * :param InInterpreter: interpreter instance
 * :param InNode: node instance
 * :return: error code
 */
SACHI_PUBLIC(int) SachiInterpreter_UnloadNode(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode);

/**
 * Find a node known by the interpreter.
 *
 * :param InInterpreter: interpreter instance
 * :param InPath: path where node is located
 * :return: matching node or NULL
 */
SACHI_PUBLIC(Sachi_Node*) SachiInterpreter_FindNode(Sachi_Interpreter* InInterpreter, const char* InPath);

/**
 * Create a new node instance in the current interpreter stack frame.
 *
 * `InPath` is the absolute path to node location.
 * `InName` is the name of new node instance.
 *
 * :param InInterpreter: interpreter instance
 * :param InPath: path where node is located
 * :param InName: name of node instance
 * :return: error code
 */
SACHI_PUBLIC(int) SachiInterpreter_InstantiateNode(Sachi_Interpreter* InInterpreter, const char* InPath, const char* InName);

SACHI_PUBLIC(int) Sachi_RunFlow(Sachi_Interpreter* InInterpreter, Sachi_Flow* InFlow);

#ifdef __cplusplus
}
#endif

#endif
