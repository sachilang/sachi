#ifndef SACHI_PINMETADATA_H
#define SACHI_PINMETADATA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sachi/config.h"
#include "sachi/posix.h"

#define SACHI_PINMODE_EXEC 0x1
#define SACHI_PINMODE_VALUE 0x2
#define SACHI_PINSIDE_IN 0x1
#define SACHI_PINSIDE_OUT 0x2

#define Sachi_PinMode unsigned char
#define Sachi_PinSide unsigned char

/**
 * Metadata of a pin.
 *
 * This is how a pin is described internally and externally.
 */
typedef struct _Sachi_PinMetadata
{
    const char* Name; // Unique name to reference this pin
    Sachi_PinMode Mode;
    Sachi_PinSide Side;
} Sachi_PinMetadata;

#ifdef __cplusplus
}
#endif

#endif