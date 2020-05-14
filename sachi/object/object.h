#ifndef SACHI_OBJECT_OBJECT_H
#define SACHI_OBJECT_OBJECT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
	
typedef struct _Sachi_Interpreter Sachi_Interpreter;
typedef struct _Sachi_Object Sachi_Object;
typedef struct _Sachi_NodeDef Sachi_NodeDef;

typedef Sachi_Object* (*SachiObject_New)(Sachi_Interpreter*);
typedef void (*SachiObject_Delete)(Sachi_Object*);

typedef struct _Sachi_Dict Sachi_Dict;

typedef struct _Sachi_ObjectType
{
	char* Name; // Unique name used to reference this type
	SachiObject_New New; // Constructor
	SachiObject_Delete Delete; // Destructor
	Sachi_NodeDef* Nodes; // Nodes/methods
} Sachi_ObjectType;

#define SACHI_OBJECT_HEADER \
	Sachi_ObjectType* Type; \
	Sachi_Interpreter* Interpreter;

typedef struct _Sachi_Object
{
	SACHI_OBJECT_HEADER
} Sachi_Object;

#ifdef __cplusplus
}
#endif

#endif