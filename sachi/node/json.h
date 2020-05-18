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

#ifdef __cplusplus
}
#endif

#endif