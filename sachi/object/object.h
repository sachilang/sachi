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

/**
 * Base type of all objects.
 */
typedef struct _Sachi_ObjectType Sachi_ObjectType;
typedef struct _Sachi_ObjectType
{
	char* Name; // Unique name used to reference this type
    sachi_size_t SizeOf; // Result of sizeof(type)
	Sachi_ObjectType* Base; // Base type
	SachiObject_New New; // Constructor
	SachiObject_Delete Delete; // Destructor
	Sachi_NodeDef* Nodes; // Nodes/methods
	SachiObject_Hash Hash; // Hash function
} Sachi_ObjectType;

/**
 * Initialize an object type.
 *
 * This correctly setup the inheritance if any.
 *
 * :param InType: type to initialize
 * :return: error code
 */
SACHI_PUBLIC(int) Sachi_InitType(Sachi_ObjectType* InType);

/**
 * Count references to an object.
 *
 * Used for garbage collection.
 */
typedef struct _Sachi_RefCounter
{
	sachi_size_t Counter;
} Sachi_RefCounter;

/**
 * Increment the reference counter of an object.
 *
 * :param InObject: object instance
 */
SACHI_PUBLIC(void) Sachi_IncRef(Sachi_Object* InObject);

/**
 * Decrement the reference counter of an object.
 *
 * This deallocate the object when the counter reachs zero.
 *
 * This may fail if object's type or destructor are not set correctly.
 *
 * :param InObject: object instance
 * :return: error code
 */
SACHI_PUBLIC(int) Sachi_DecRef(Sachi_Object* InObject);

/**
 * Base structure of all allocated objects.
 */
#define SACHI_OBJECT_HEADER \
	Sachi_RefCounter RefCounter; \
	Sachi_ObjectType* Type; \
	Sachi_Interpreter* Interpreter;

typedef struct _Sachi_Object
{
	SACHI_OBJECT_HEADER
} Sachi_Object;

/**
 * Initialize an object.
 *
 * This correctly setup the base structure.
 *
 * Must be called from the object's constructor right after
 * allocating the object in memory.
 *
 * :param InInterpreter: interpreter instance
 * :param InObject: object to initialize
 * :param InType: object's type
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewObject(Sachi_Interpreter* InInterpreter, Sachi_ObjectType* InType);

/**
 * Clean and free up an object.
 *
 * Must be called from the object's destructor, this
 * deallocate the memory.
 *
 * :param InObject: object to delete
 */
SACHI_PUBLIC(void) Sachi_DeleteObject(Sachi_Object* InObject);

/**
 * Called when an object is NULL.
 *
 * :param InInterpreter: interpreter instance
 */
SACHI_PUBLIC(void) SachiInterpreter_NullObjectError(Sachi_Interpreter* InInterpreter);

#ifdef __cplusplus
}
#endif

#endif