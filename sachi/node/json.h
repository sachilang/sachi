#ifndef SACHI_NODE_JSON_H
#define SACHI_NODE_JSON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"
	
SACHI_PUBLIC(Sachi_Object*) Sachi_NewJSON(Sachi_Interpreter* InInterpreter);
SACHI_PUBLIC(void) Sachi_DeleteJSON(Sachi_Object* InObject);

/**
 * Parse a JSON string to dict.
 *
 * :param InInterpreter: instance of interpreter
 * :param InObject: string to parse
 * :param OutObject: dict object
 * :return: error code
 */
SACHI_PUBLIC(int) SachiJSON_Loads(Sachi_Object* InInterpreter, Sachi_Object* InObject, Sachi_Object** OutObject);

/**
 * Parse a JSON string to dict.
 *
 * :param InInterpreter: instance of interpreter
 * :param InBuffer: string to parse
 * :param OutObject: dict object
 * :return: error code
 */
SACHI_PUBLIC(int) SachiJSON_LoadsFromBuffer(Sachi_Object* InInterpreter, const char* InBuffer, Sachi_Object** OutObject);

/**
 * Parse a JSON string to dict.
 *
 * :param InInterpreter: instance of interpreter
 * :param InBuffer: string to parse
 * :param InLength: string length
 * :param OutObject: dict object
 * :return: error code
 */
SACHI_PUBLIC(int) SachiJSON_LoadsFromBufferAndLength(Sachi_Object* InInterpreter, const char* InBuffer, sachi_size_t InLength, Sachi_Object** OutObject);

#ifdef __cplusplus
}
#endif

#endif