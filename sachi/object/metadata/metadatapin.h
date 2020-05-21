#ifndef SACHI_OBJECT_METADATA_METADATAPIN_H
#define SACHI_OBJECT_METADATA_METADATAPIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"
#include "sachi/object/object.h"

typedef struct _Sachi_Interpreter Sachi_Interpreter;
typedef struct _Sachi_PinMetadata Sachi_PinMetadata;

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPinFromMetadata(Sachi_Interpreter* InInterpreter, Sachi_PinMetadata* InMetadata);

SACHI_PUBLIC(int) SachiPin_InitFromMetadata(Sachi_Object* InObject, Sachi_PinMetadata* InMetadata);

#ifdef __cplusplus
}
#endif

#endif