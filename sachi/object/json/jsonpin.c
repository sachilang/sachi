#include "sachi/object/json/jsonpin.h"
#include "sachi/object/pin.h"
#include "sachi/object/string.h"
#include "sachi/object/dict.h"
#include "sachi/object/interpreter.h"
#include "sachi/pinmetadata.h"

SACHI_PUBLIC(Sachi_Object*) Sachi_NewPinFromJSON(Sachi_Interpreter* InInterpreter, Sachi_Object* InDict)
{
	Sachi_Object* Pin = Sachi_NewPin(InInterpreter);
	if (!Pin)
	{
		return NULL;
	}

	if (SachiPin_InitFromJSON(Pin, InDict) != SACHI_OK)
	{
		Sachi_DecRef(Pin);
		return NULL;
	}

	return Pin;
}

SACHI_PUBLIC(int) SachiPin_InitFromJSON(Sachi_Object* InObject, Sachi_Object* InDict)
{
	Sachi_Object* Name = NULL;
	Sachi_Object* Mode = NULL;
	Sachi_Object* Side = NULL;
	if (SachiDict_GetItemFromBuffer(InDict, "name", &Name) != SACHI_OK ||
		SachiDict_GetItemFromBuffer(InDict, "mode", &Mode) != SACHI_OK ||
		SachiDict_GetItemFromBuffer(InDict, "side", &Side) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!Name)
	{
		SachiInterpreter_SetErrorMessage(InObject, "pin name is required");
		return SACHI_ERROR;
	}

	if (!Mode)
	{
		SachiInterpreter_SetErrorMessage(InObject, "pin mode is required");
		return SACHI_ERROR;
	}

	if (!Side)
	{
		SachiInterpreter_SetErrorMessage(InObject, "pin side is required");
		return SACHI_ERROR;
	}

	if (!Sachi_IsString(Name))
	{
		SachiInterpreter_SetErrorMessage(InObject, "invalid type for pin name, string expected");
		return SACHI_ERROR;
	}

	if (SachiPin_SetName(InObject, Name) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!Sachi_IsString(Mode))
	{
		SachiInterpreter_SetErrorMessage(InObject, "invalid type for pin mode, string expected");
		return SACHI_ERROR;
	}

	if (SachiPin_SetMode(InObject, Mode) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!Sachi_IsString(Side))
	{
		SachiInterpreter_SetErrorMessage(InObject, "invalid type for pin side, string expected");
		return SACHI_ERROR;
	}

	if (SachiPin_SetSide(InObject, Side) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}