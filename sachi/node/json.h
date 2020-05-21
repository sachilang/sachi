#ifndef SACHI_NODE_JSON_H
#define SACHI_NODE_JSON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"
#include "sachi/nodemetadata.h"

extern Sachi_NodeMetadata Sachi_JSONNodeMetadata;

/**
 * Parse a JSON string to dict.
 *
 * :param InInterpreter: instance of interpreter
 * :param InObject: string to parse
 * :param OutObject: dict object
 * :return: error code
 */
SACHI_PUBLIC(int) SachiJSON_Loads(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object** OutObject);

/**
 * Parse a JSON string to dict.
 *
 * :param InInterpreter: instance of interpreter
 * :param InBuffer: string to parse
 * :param OutObject: dict object
 * :return: error code
 */
SACHI_PUBLIC(int) SachiJSON_LoadsFromBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer, Sachi_Object** OutObject);

/**
 * Parse a JSON string to dict.
 *
 * :param InInterpreter: instance of interpreter
 * :param InBuffer: string to parse
 * :param InLength: string length
 * :param OutObject: dict object
 * :return: error code
 */
SACHI_PUBLIC(int) SachiJSON_LoadsFromBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutObject);

SACHI_PUBLIC(int) SachiJSON_Load(Sachi_Interpreter* InInterpreter, Sachi_Object* InObject, Sachi_Object** OutObject);
SACHI_PUBLIC(int) SachiJSON_LoadWithBuffer(Sachi_Interpreter* InInterpreter, const char* InBuffer, Sachi_Object** OutObject);
SACHI_PUBLIC(int) SachiJSON_LoadWithBufferAndLength(Sachi_Interpreter* InInterpreter, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutObject);

#ifdef __cplusplus
}
#endif

#endif