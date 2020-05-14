#ifndef SACHI_OBJECT_PIN_H
#define SACHI_OBJECT_PIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;

extern Sachi_ObjectType Sachi_PinType;

#define Sachi_CheckPin(o) (o->Type == &Sachi_PinType)

#define SACHI_PINMODE_EXEC 0x1
#define SACHI_PINMODE_VALUE 0x2
#define SACHI_PINSIDE_IN 0x1
#define SACHI_PINSIDE_OUT 0x2

#define Sachi_PinMode unsigned char
#define Sachi_PinSide unsigned char
    
/**
 * Definition of a pin.
 *
 * This is how a pin is described externally.
 */
typedef struct _Sachi_PinDef
{
    const char* Label; // Display only label
    const char* Name; // Unique name to reference this pin
    Sachi_PinMode Mode;
    Sachi_PinSide Side;
    const char* Type;
    const char* Default;
} Sachi_PinDef;

/**
 * Create a new pin with an empty definition.
 *
 * :param InInterpreter: interpreter instance
 * :return: new pin
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewPin(Sachi_Interpreter* InInterpreter);

/**
 * Delete a pin.
 *
 * :param InObject: pin to delete
 */
SACHI_PUBLIC(void) Sachi_DeletePin(Sachi_Object* InObject);

/**
 * Get the definition of this pin.
 *
 * :param InObject: instance
 * :return: definition
 */
SACHI_PUBLIC(Sachi_PinDef*) SachiPin_GetDefition(Sachi_Object* InObject);

/**
 * Set the definition of this pin.
 *
 * :param InObject: instance
 * :param InDefinition: new definition
 */
SACHI_PUBLIC(void) SachiPin_SetDefition(Sachi_Object* InObject, Sachi_PinDef* InDefinition);

/**
 * Get the definition of this pin.
 *
 * :param InObject: instance
 * :return: definition
 */
SACHI_PUBLIC(const char*) SachiPin_GetName(Sachi_Object* InObject);

#ifdef __cplusplus
}
#endif

#endif