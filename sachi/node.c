#include "sachi/node.h"
#include "sachi/posix.h"

#define SACHI_NEWGENERIC(var, type, init) \
type* var = sachi_malloc(sizeof(type)); \
init(var)

#define SACHI_NEWGENERICWITHLENGTH(var, type, init, length) \
type* var = sachi_malloc(sizeof(type) * length); \
for (sachi_size_t I = 0; I < length; ++I) \
{ \
	init(&var[I]); \
}

#define SACHI_NEWGENERIC_RETURN(type, init) \
SACHI_NEWGENERIC(O, type, init); \
return O

#define SACHI_NEWGENERICWITHLENGTH_RETURN(type, init, length) \
SACHI_NEWGENERICWITHLENGTH(O, type, init, length); \
return O

#define SACHI_GENERATENEW(name) \
SACHI_PUBLIC(Sachi_##name *) Sachi_New##name() \
{ \
	SACHI_NEWGENERIC_RETURN(Sachi_##name, Sachi_Init##name); \
} \
 \
SACHI_PUBLIC(Sachi_##name *) Sachi_New##name##WithLength(sachi_size_t InLength) \
{ \
	SACHI_NEWGENERICWITHLENGTH_RETURN(Sachi_##name, Sachi_Init##name, InLength); \
}

#define SACHI_GENERATEDELETE(name) \
SACHI_PUBLIC(void) Sachi_Delete##name(Sachi_##name * InObject) \
{ \
	if (InObject) \
	{ \
		Sachi_Reset##name; \
		sachi_free(InObject); \
	} \
} \
 \
SACHI_PUBLIC(void) Sachi_Delete##name##WithLength(Sachi_##name * InObject, sachi_size_t InLength) \
{ \
	if (InObject) \
	{ \
		for (sachi_size_t I = 0; I < InLength; ++I) \
		{ \
			Sachi_Reset##name(&InObject[I]); \
		} \
	} \
	sachi_free(InObject); \
}

SACHI_GENERATENEW(Pin);
SACHI_GENERATEDELETE(Pin);
SACHI_GENERATENEW(Node);
SACHI_GENERATEDELETE(Node);

SACHI_PUBLIC(void) Sachi_InitPin(Sachi_Pin* InObject)
{
	InObject->Label = NULL;
	InObject->Name = NULL;
	InObject->Mode = 0;
	InObject->Side = 0;
	InObject->Type = NULL;
	InObject->Default = NULL;
}

SACHI_PUBLIC(void) Sachi_ResetPin(Sachi_Pin* InObject)
{
	sachi_free(InObject->Label);
	InObject->Label = NULL;
	sachi_free(InObject->Name);
	InObject->Name = NULL;
	sachi_free(InObject->Type);
	InObject->Type = NULL;
	sachi_free(InObject->Default);
	InObject->Default = NULL;
}

SACHI_PUBLIC(int) Sachi_PinModeFromString(const char* InStr, Sachi_PinMode* OutValue)
{
	if (sachi_strcmp(InStr, "exec") == 0)
	{
		*OutValue = SACHINODE_PINMODE_EXEC;
	}
	else if (sachi_strcmp(InStr, "value") == 0)
	{
		*OutValue = SACHINODE_PINMODE_VALUE;
	}
	else
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(int) Sachi_PinSideFromString(const char* InStr, Sachi_PinSide* OutValue)
{
	if (sachi_strcmp(InStr, "in") == 0)
	{
		*OutValue = SACHINODE_PINSIDE_IN;
	}
	else if (sachi_strcmp(InStr, "out") == 0)
	{
		*OutValue = SACHINODE_PINSIDE_OUT;
	}
	else
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

SACHI_PUBLIC(void) Sachi_InitNode(Sachi_Node* InObject)
{
	InObject->Name = NULL;
	InObject->Nodes = NULL;
	InObject->NumNodes = 0;
	InObject->Pins = NULL;
	InObject->NumPins = 0;
	InObject->Flow = NULL;
}

SACHI_PUBLIC(void) Sachi_ResetNode(Sachi_Node* InObject)
{
	sachi_free(InObject->Name);
	InObject->Name = NULL;
	InObject->Nodes = NULL;
	InObject->NumNodes = 0;
	if (InObject->NumPins > 0)
	{
		Sachi_DeletePinWithLength(InObject->Pins, InObject->NumPins);
		InObject->Pins = NULL;
		InObject->NumPins = 0;
	}
	sachi_free(InObject->Flow);
	InObject->Flow = NULL;
}

/*static int Sachi_GetMain(Sachi_Node* InNode, Sachi_MainNode* OutNode)
{
	if (!InNode)
	{
		return SACHI_ERROR;
	}

	if (InNode->NumInputPins != 2)
	{
		return SACHI_ERROR;
	}

	if (InNode->NumOutputPins != 2)
	{
		return SACHI_ERROR;
	}

	OutNode->ExecInputPin = NULL;
	OutNode->ValueInputPin = NULL;
	OutNode->ExecOutputPin = NULL;
	OutNode->ValueOutputPin = NULL;

	Sachi_NodePin* Pin = InNode->InputPins;
	for (sachi_size_t I = 0; I < InNode->NumInputPins; ++I)
	{
		if (Pin->Type == Sachi_NodePIN_EXEC)
		{
			OutNode->ExecInputPin = Pin;
		}
		else if (Pin->Type == Sachi_NodePIN_VALUE)
		{
			OutNode->ValueInputPin = Pin;
		}
		++Pin;
	}

	Pin = InNode->OutputPins;
	for (sachi_size_t I = 0; I < InNode->NumOutputPins; ++I)
	{
		if (Pin->Type == Sachi_NodePIN_EXEC)
		{
			OutNode->ExecOutputPin = Pin;
		}
		else if (Pin->Type == Sachi_NodePIN_VALUE)
		{
			OutNode->ValueOutputPin = Pin;
		}
		++Pin;
	}

	if (!OutNode->ExecInputPin)
	{
		return SACHI_ERROR;
	}

	if (!OutNode->ValueInputPin)
	{
		return SACHI_ERROR;
	}

	if (!OutNode->ExecOutputPin)
	{
		return SACHI_ERROR;
	}

	if (!OutNode->ValueOutputPin)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}*/

#ifdef SACHI_LOG_ENABLED
SACHI_PUBLIC(void) Sachi_PrintNode(Sachi_Node* InNode)
{
	sachi_printf("- name: %s\n", InNode->Name);
	sachi_printf("- pins:\n");
	if (InNode->NumPins > 0)
	{
		Sachi_Pin* Pin = InNode->Pins;
		for (sachi_size_t I = 0; I < InNode->NumPins; ++I)
		{
			sachi_printf("  - name: %s\n", Pin->Name);
			sachi_printf("  - mode: %d\n", (int)Pin->Mode);
			sachi_printf("  - side: %d\n", (int)Pin->Side);
			Pin++;
		}
	}
}
#endif