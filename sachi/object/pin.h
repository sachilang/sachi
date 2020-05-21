#ifndef SACHI_OBJECT_PIN_H
#define SACHI_OBJECT_PIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"
#include "sachi/pinmetadata.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;

/**
 * Representation of a pin's metadata in the language.
 */
extern Sachi_ObjectType Sachi_PinType;

#define Sachi_CheckPin(o) (o->Type == &Sachi_PinType)

/**
 * Create a new pin with empty metadata.
 *
 * :param InInterpreter: interpreter instance
 * :return: new pin
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewPin(Sachi_Interpreter* InInterpreter);

/**
 * Create a new pin with an empty definition.
 *
 * Pin's metadata are initialized from dict.
 *
 * :param InInterpreter: interpreter instance
 * :param InDict: dict containing metadata
 * :return: new pin
 */
SACHI_PUBLIC(Sachi_Object*) Sachi_NewPinFromDict(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict);

/**
 * Delete a pin.
 *
 * :param InObject: pin to delete
 */
SACHI_PUBLIC(void) Sachi_DeletePin(Sachi_Object* InObject);

/**
 * Set the metadata of this pin.
 *
 * :param InObject: instance
 * :param InDict: dict containing metadata
 * :return: error code
 */
SACHI_PUBLIC(int) SachiPin_InitFromDict(Sachi_Object* InObject, Sachi_Object* InDict);

SACHI_PUBLIC(const char*) SachiPin_GetName(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiPin_SetName(Sachi_Object* InObject, Sachi_Object* InName);
SACHI_PUBLIC(int) SachiPin_SetNameFromBuffer(Sachi_Object* InObject, const char* InBuffer);
SACHI_PUBLIC(int) SachiPin_SetNameFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength);
SACHI_PUBLIC(Sachi_PinSide) SachiPin_GetSide(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiPin_SetSide(Sachi_Object* InObject, Sachi_Object* InSide);
SACHI_PUBLIC(void) SachiPin_SetSideFromValue(Sachi_Object* InObject, Sachi_PinSide InSide);
SACHI_PUBLIC(Sachi_PinMode) SachiPin_GetMode(Sachi_Object* InObject);
SACHI_PUBLIC(int) SachiPin_SetMode(Sachi_Object* InObject, Sachi_Object* InMode);
SACHI_PUBLIC(void) SachiPin_SetModeFromValue(Sachi_Object* InObject, Sachi_PinMode InMode);

#ifdef __cplusplus
}
#endif

#endif