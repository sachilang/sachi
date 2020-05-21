#include "sachi/object/metadata/metadatapin.h"
#include "sachi/object/pin.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPinFromMetadata(Sachi_Interpreter* InInterpreter, Sachi_PinMetadata* InMetadata)
{
	Sachi_Object* Value = Sachi_NewPin(InInterpreter);
	if (Value)
	{
		SachiPin_InitFromMetadata(Value, InMetadata);
	}

	return Value;
}

SACHI_PUBLIC(int) SachiPin_InitFromMetadata(Sachi_Object* InObject, Sachi_PinMetadata* InMetadata)
{
	if (SachiPin_SetNameFromBuffer(InObject, InMetadata->Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	SachiPin_SetModeFromValue(InObject, InMetadata->Mode);
	SachiPin_SetSideFromValue(InObject, InMetadata->Side);

	return SACHI_OK;
}