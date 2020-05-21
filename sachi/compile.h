#ifndef SACHI_COMPILE_H
#define SACHI_COMPILE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"

/**
 * Nodal version of a program.
 */
typedef struct _Sachi_Node Sachi_Node;

/**
 * Compiled version of a node.
 */
typedef struct _Sachi_Program Sachi_Program;

/**
 * Compile a node into a program.
 *
 * :param InNode: node to compile
 * :param OutProgram: compiled program
 * :return: error code
 */
SACHI_PUBLIC(int) Sachi_Compile(Sachi_Node* InNode, Sachi_Program** OutProgram);

#ifdef __cplusplus
}
#endif

#endif