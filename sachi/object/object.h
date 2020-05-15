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
typedef LONG (*SachiObject_Hash)(Sachi_Object*);

typedef struct _Sachi_Dict Sachi_Dict;

typedef struct _Sachi_ObjectType Sachi_ObjectType;
typedef struct _Sachi_ObjectType
{
	char* Name; // Unique name used to reference this type
	Sachi_ObjectType* Base; // Base type
	SachiObject_New New; // Constructor
	SachiObject_Delete Delete; // Destructor
	Sachi_NodeDef* Nodes; // Nodes/methods
	SachiObject_Hash Hash; // Hash function
} Sachi_ObjectType;

#define SACHI_OBJECT_HEADER \
	Sachi_ObjectType* Type; \
	Sachi_Interpreter* Interpreter;

typedef struct _Sachi_Object
{
	SACHI_OBJECT_HEADER
} Sachi_Object;

/**
 * Initialize an object type.
 *
 * This correctly setup the inheritance if any.
 *
 * :param InType: type to initialize
 * :return: error code
 */
SACHI_PUBLIC(int) Sachi_InitializeType(Sachi_ObjectType* InType);

#ifdef __cplusplus
}
#endif

#endif