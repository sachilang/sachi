#ifndef SACHI_NODEMETADATA_H
#define SACHI_NODEMETADATA_H

#ifdef __cplusplus
extern "C"
{
#endif
     
#include "sachi/config.h"
#include "sachi/posix.h"

typedef struct _Sachi_Object Sachi_Object;
typedef struct _Sachi_Interpreter Sachi_Interpreter;
typedef struct _Sachi_PinMetadata Sachi_PinMetadata;
typedef struct _Sachi_NodeMetadata Sachi_NodeMetadata;

/**
 * C implementation of a node call.
 *
 * Parameter `InInputExecPin` allow the node implementation
 * to know from which exec pin we enter this node.
 *
 * Parameter `OutOutputExecPin` allow caller to know from
 * which exec pin we exit this node.
 *
 * Arguments are passed as keyword arguments to let the node
 * implementation access them based on pins names.
 *
 * Results are passed as keyword arguments to let the caller
 * access them based on pins names.
 *
 * Given a node defined as:
 *
 * .. code-block:: c
 *
 *     ---------------------------------
 *   -| execin: exec         exec :true |-
 *   -| condition: bool     exec :false |-
 *     ---------------------------------
 *
 * `InInputExecPin` would be a pointer to the `execin` pin.
 * `InKwArgs` would contain a `condition=value` argument.
 *
 * Depending on the `condition` input value, `OutOutputExecPin`
 * would either be a pointer to `true` pin or `false` pin.
 *
 * Given a node defined as:
 *
 * .. code-block:: c
 *
 *     ---------------------------
 *   -| a: number     number :out |-
 *   -| b: number                 |-
 *     ---------------------------
 *
 * Both `InInputExecPin` and `OutOutputExecPin` would be NULL.
 * `InKwArgs` would contain both `a=value` and `b=value` arguments.
 *
 * :param InInterpreter: instance of interpreter
 * :param InObject: instance of the node being called
 * :param InInputExecPin: node's input exec pin
 * :param InKwArgs: keyword based arguments
 * :param OutOutputExecPin: node's output exec pin
 * :param OutKwResults: keyword based results
 * :return: error code
 */
typedef int (*Sachi_CFunc)(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults);

/**
 * Metadata of a node.
 *
 * This is how a node is described internally and externally.
 */
typedef struct _Sachi_NodeMetadata
{
	const char* Name; // Unique name to reference this node
	Sachi_CFunc Func; // C function implementing this node
    Sachi_PinMetadata* Pins; // Pins
    Sachi_NodeMetadata* Nodes; // Child nodes
} Sachi_NodeMetadata;

#ifdef __cplusplus
}
#endif

#endif