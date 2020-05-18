#ifndef SACHI_OBJECT_NODE_H
#define SACHI_OBJECT_NODE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;
typedef struct _Sachi_Node Sachi_Node;
typedef struct _Sachi_PinDef Sachi_PinDef;
typedef struct _Sachi_NodeDef Sachi_NodeDef;

extern Sachi_ObjectType Sachi_NodeType;

#define Sachi_CheckNode(o) (o->Type == &Sachi_NodeType)

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
 * Definition of a node.
 *
 * This is how a node is described externally.
 */
typedef struct _Sachi_NodeDef
{
	const char* Name; // Unique name to reference this node
	Sachi_CFunc Func; // C function implementing this node
    Sachi_PinDef* Pins; // Pins definitions
    Sachi_NodeDef* Children; // Child nodes definitions
} Sachi_NodeDef;

/**
 * Create a new node with an empty definition.
 *
 * :param InInterpreter: interpreter instance
 * :return: new node
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewNode(Sachi_Interpreter* InInterpreter);

/**
 * Delete a node.
 *
 * :param InObject: node to delete
 */
SACHI_PUBLIC(void) Sachi_DeleteNode(Sachi_Object* InObject);

/**
 * Get the definition of this node.
 *
 * :param InObject: instance
 * :return: definition
 */
SACHI_PUBLIC(Sachi_NodeDef*) SachiNode_GetDefition(Sachi_Object* InObject);

/**
 * Set the definition of this node.
 *
 * Allocate pins depending on definition.
 *
 * :param InObject: instance
 * :param InDefinition: new definition
 * :return: error code
 */
SACHI_PUBLIC(int) SachiNode_SetDefition(Sachi_Object* InObject, Sachi_NodeDef* InDefinition);

/**
 * Get the name of this node.
 *
 * :param InObject: instance
 * :return: name
 */
SACHI_PUBLIC(const char*) SachiNode_GetName(Sachi_Object* InObject);

/**
 * Get the pins of this node.
 *
 * :param InObject: instance
 * :return: list of pins
 */
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPins(Sachi_Object* InObject);

/**
 * Get the pins of this node.
 *
 * :param InObject: instance
 * :param InName: name to find
 * :return: list of pins
 */
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetPin(Sachi_Object* InObject, const char* InName);

/**
 * Get the pins of this node.
 *
 * :param InObject: instance
 * :return: list of pins
 */
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetChildren(Sachi_Object* InObject);

/**
 * Get the pins of this node.
 *
 * :param InObject: instance
 * :param InName: name to find
 * :return: list of pins
 */
SACHI_PUBLIC(Sachi_Object*) SachiNode_GetChild(Sachi_Object* InObject, const char* InName);

/**
 * Call a function of this node.
 *
 * :param InObject: instance
 * :return: error code
 */
SACHI_PUBLIC(int) SachiNode_Call(Sachi_Object* InObject, Sachi_Object* InInputExecPin, Sachi_Object* InKwArgs, Sachi_Object** OutOutputExecPin, Sachi_Object* OutKwResults);

#ifdef __cplusplus
}
#endif

#endif