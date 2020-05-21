#include "sachi/object/pin.h"
#include "sachi/object/interpreter.h"
#include "sachi/object/string.h"
#include "sachi/object/dict.h"
#include "sachi/pinmetadata.h"
#include "sachi/sachi.h"

typedef struct _Sachi_Pin
{
	SACHI_OBJECT_HEADER
	const char* Name;
	Sachi_PinMode Mode;
	Sachi_PinSide Side;
} Sachi_Pin;

static Sachi_Object* _Sachi_NewPin(Sachi_Interpreter* InInterpreter)
{
	return Sachi_NewPin(InInterpreter);
}

static void _Sachi_DeletePin(Sachi_Object* InObject)
{
	Sachi_DeletePin(InObject);
}

Sachi_ObjectType Sachi_PinType = {
	"pin",
	sizeof(Sachi_Pin),
	NULL, // base
	_Sachi_NewPin,
	_Sachi_DeletePin,
	NULL, // nodes
	NULL, // hash
	NULL, // to string
};

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPin(Sachi_Interpreter* InInterpreter)
{
	Sachi_Pin* Value = (Sachi_Pin*)Sachi_NewObject(InInterpreter, &Sachi_PinType);
	if (!Value)
	{
		return NULL;
	}

	Value->Name = NULL;
	Value->Mode = SACHI_PINMODE_EXEC;
	Value->Side = SACHI_PINSIDE_IN;

	return (Sachi_Object*)Value;
}

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPinFromDict(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict)
{
	Sachi_Object* Value = Sachi_NewPin(InInterpreter);
	if (Value)
	{
		SachiPin_InitFromDict(Value, InDict);
	}

	return Value;
}

SACHI_PUBLIC(void) Sachi_DeletePin(Sachi_Object* InObject)
{
	if (!InObject)
	{
		return;
	}

	Sachi_Pin* Pin = (Sachi_Pin*)InObject;
	sachi_free(Pin->Name);
	Pin->Name = NULL;
	Pin->Mode = SACHI_PINMODE_EXEC;
	Pin->Side = SACHI_PINSIDE_IN;
	Sachi_DeleteObject(InObject);
}

SACHI_PUBLIC(int) SachiPin_InitFromDict(Sachi_Object* InObject, Sachi_Object* InDict)
{
	Sachi_Object* Name = NULL;
	Sachi_Object* Mode = NULL;
	Sachi_Object* Side = NULL;

	if (SachiDict_GetItemFromBuffer(InDict, "name", &Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_GetItemFromBuffer(InDict, "mode", &Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiDict_GetItemFromBuffer(InDict, "side", &Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiPin_SetName(InObject, Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiPin_SetMode(InObject, Mode) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiPin_SetSide(InObject, Side) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(const char*) SachiPin_GetName(Sachi_Object* InObject)
{
	return ((Sachi_Pin*)InObject)->Name;
}

SACHI_PUBLIC(int) SachiPin_SetName(Sachi_Object* InObject, Sachi_Object* InName)
{
	return SachiPin_SetNameFromBufferAndLength(InObject, SachiString_Data(InName), SachiString_Size(InName));
}

SACHI_PUBLIC(int) SachiPin_SetNameFromBuffer(Sachi_Object* InObject, const char* InBuffer)
{
	return SachiPin_SetNameFromBufferAndLength(InObject, InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(int) SachiPin_SetNameFromBufferAndLength(Sachi_Object* InObject, const char* InBuffer, sachi_size_t InLength)
{
	const char* Buffer = NULL;
	if (Sachi_CopyBuffer(InBuffer, &Buffer) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	Sachi_Pin* Pin = (Sachi_Pin*)InObject;
	sachi_free(Pin->Name);
	Pin->Name = Buffer;

	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_PinSide) SachiPin_GetSide(Sachi_Object* InObject)
{
	return ((Sachi_Pin*)InObject)->Side;
}

SACHI_PUBLIC(int) SachiPin_SetSide(Sachi_Object* InObject, Sachi_Object* InSide)
{
	const char* Buffer = SachiString_Data(InSide);
	if (sachi_strcmp(Buffer, "in") == 0)
	{
		((Sachi_Pin*)InObject)->Side = SACHI_PINSIDE_IN;
	}
	else if (sachi_strcmp(Buffer, "out") == 0)
	{
		((Sachi_Pin*)InObject)->Side = SACHI_PINSIDE_OUT;
	}
	else
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(void) SachiPin_SetSideFromValue(Sachi_Object* InObject, Sachi_PinSide InSide)
{
	((Sachi_Pin*)InObject)->Side = InSide;
}

SACHI_PUBLIC(Sachi_PinMode) SachiPin_GetMode(Sachi_Object* InObject)
{
	return ((Sachi_Pin*)InObject)->Mode;
}

SACHI_PUBLIC(int) SachiPin_SetMode(Sachi_Object* InObject, Sachi_Object* InMode)
{
	const char* Buffer = SachiString_Data(InMode);
	if (sachi_strcmp(Buffer, "exec") == 0)
	{
		((Sachi_Pin*)InObject)->Mode = SACHI_PINMODE_EXEC;
	}
	else if (sachi_strcmp(Buffer, "value") == 0)
	{
		((Sachi_Pin*)InObject)->Mode = SACHI_PINMODE_VALUE;
	}
	else
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(void) SachiPin_SetModeFromValue(Sachi_Object* InObject, Sachi_PinMode InMode)
{
	((Sachi_Pin*)InObject)->Mode = InMode;
}