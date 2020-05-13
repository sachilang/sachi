#ifndef SACHI_PARSER_H
#define SACHI_PARSER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"

typedef struct _Sachi_Node Sachi_Node;

/**
 * Read file content to memory buffer.
 *
 * :param InFilename: path to file
 * :param OutBuffer: buffer to allocate
 * :param OutSize: size of allocated buffer
 * :return: error code
 */
SACHI_PUBLIC(int) Sachi_ReadFile(const char* InFilename, char** OutBuffer, sachi_size_t* OutSize);

/**
 * Parse a node from an external JSON file.
 *
 * :param InFilename: path to file
 * :return: parsed node on success or NULL
 */
SACHI_PUBLIC(Sachi_Node*) Sachi_Load(const char* InFilename);

/**
 * Parse a node from an in-memory JSON string.
 *
 * :param InBuffer: JSON string to parse
 * :return: parsed node on success or NULL
 */
SACHI_PUBLIC(Sachi_Node*) Sachi_Loads(const char* InBuffer);

/**
 * Parse a node from an in-memory JSON string.
 *
 * This version accept a fixed length for parsed string.
 *
 * :param InBuffer: JSON string to parse
 * :param InBufferLength: string size
 * :return: parsed node on success or NULL
 */
SACHI_PUBLIC(Sachi_Node*) Sachi_LoadsWithLength(const char* InBuffer, sachi_size_t InBufferLength);

#ifdef __cplusplus
}
#endif

#endif