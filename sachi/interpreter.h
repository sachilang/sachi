#ifndef SACHI_INTERPRETER_H
#define SACHI_INTERPRETER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
	
typedef struct _Sachi_Node Sachi_Node;
typedef struct _Sachi_Interpreter Sachi_Interpreter;

SACHI_PUBLIC(const char*) SachiInterpreter_GetErrorPtr(void);
SACHI_PUBLIC(Sachi_Interpreter*) SachiInterpreter_NewInterpreter();
SACHI_PUBLIC(int) SachiInterpreter_RunNode(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode);
SACHI_PUBLIC(int) SachiInterpreter_RunMain(Sachi_Interpreter* InInterpreter, Sachi_Node* InNode);

#ifdef __cplusplus
}
#endif

#endif
