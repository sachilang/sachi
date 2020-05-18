#include "sachi/parser.h"
#include "sachi/sachi.h"
#include "sachi/posix.h"
#include "sachi/node.h"
#include "cJSON/cJSON.h"

static void SachiParser_SetErrorNULL()
{
	Sachi_SetErrorPtr("invalid NULL pointer");
}

static void SachiParser_SetErrorJSONKeyNotFound()
{
	Sachi_SetErrorPtr("non existent JSON key");
}

static void SachiParser_SetErrorNotJSONArray()
{
	Sachi_SetErrorPtr("expected a JSON array");
}

static void SachiParser_SetErrorNotJSONString()
{
	Sachi_SetErrorPtr("expected a JSON string");
}

static void SachiParser_SetErrorMemoryAllocation()
{
	Sachi_SetErrorPtr("memory allocation failed");
}

static void SachiParser_SetErrorReadFile()
{
	Sachi_SetErrorPtr("failed to read file");
}

/**
 * Get a string value from JSON dict.
 *
 * Given a JSON dict and key, validate that the key exists and
 * return the string value or NULL.
 *
 * The string value memory stay owned by the JSON dict.
 *
 * :param InDict: JSON dict
 * :param InKey: key of child
 * :param OutValue: string value
 * :param InIsRequired: raise error if key doesn't exist
 * :return: error code
 */
static int SachiParser_GetJSONString(cJSON* InDict, const char* InKey, char** OutValue, BOOL InIsRequired)
{
	cJSON* Item = cJSON_GetObjectItem(InDict, InKey);
	if (!Item)
	{
		if (InIsRequired)
		{
			SachiParser_SetErrorJSONKeyNotFound();
			return SACHI_ERROR;
		}

		*OutValue = NULL;
		return SACHI_OK;
	}

	if (!cJSON_IsString(Item))
	{
		SachiParser_SetErrorNotJSONString();
		return SACHI_ERROR;
	}

	*OutValue = Item->valuestring;
	return SACHI_OK;
}

/**
 * Get a string value from JSON dict.
 *
 * The string value is copied to OutValue in an allocated buffer.
 *
 * :param InDict: JSON dict
 * :param InKey: key of child string
 * :param OutValue: buffer to allocate and fill
 * :param InIsRequired: raise error if key doesn't exist
 * :return: string or NULL
 */
static int SachiParser_CopyJSONString(cJSON* InDict, const char* InKey, char** OutValue, BOOL InIsRequired)
{
	char* S = NULL;
	if (SachiParser_GetJSONString(InDict, InKey, &S, InIsRequired) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (!S)
	{
		*OutValue = NULL;
		return SACHI_OK;
	}

	sachi_size_t Size = sachi_strlen(S);
	char* Buffer = (char*)sachi_malloc(sizeof(char) * Size);
	if (!Buffer)
	{
		SachiParser_SetErrorMemoryAllocation();
		return SACHI_ERROR;
	}

	sachi_memcpy(Buffer, S, Size);
	*OutValue = Buffer;

	return SACHI_OK;
}

// Copy an array of JSON items.
// New = a lambda creating a new generic item
// Copy = a lambda copying a generic item from JSON
static int SachiParser_ArrayFromJSON(cJSON* InDict, const char* InName, void** OutObjects, sachi_size_t* OutSize, void* (*New)(sachi_size_t), int(*Copy)(cJSON*, void*, sachi_size_t), void (*Delete)(void*, sachi_size_t), BOOL InIsRequired)
{
	cJSON* List = cJSON_GetObjectItem(InDict, InName);
	if (!List)
	{
		if (InIsRequired)
		{
			SachiParser_SetErrorJSONKeyNotFound();
			return SACHI_ERROR;
		}
		else
		{
			*OutObjects = NULL;
			*OutSize = 0;
			return SACHI_OK;
		}
	}

	if (!cJSON_IsArray(List))
	{
		SachiParser_SetErrorNotJSONArray();
		return SACHI_ERROR;
	}

	sachi_size_t Size = 0;
	void* Objects = NULL;

	if (List)
	{
		Size = cJSON_GetArraySize(List);
		Objects = New(Size);
		if (!Objects)
		{
			SachiParser_SetErrorMemoryAllocation();
			goto fail;
		}
	}

	for (sachi_size_t I = 0; I < Size; ++I)
	{
		cJSON* Item = cJSON_GetArrayItem(List, I);
		if (Copy(Item, Objects, I) != SACHI_OK)
		{
			goto fail;
		}
	}

	*OutSize = Size;
	*OutObjects = Objects;
	return SACHI_OK;

fail:
	if (Objects)
	{
		Delete(Objects, Size);
	}
	return SACHI_ERROR;
}

/**
 * Copy pin data from JSON dict.
 *
 * :param InDict: JSON dict
 * :param OutPin: pin
 * :return: error code
 */
static int SachiParser_PinFromJSON(cJSON* InDict, Sachi_Pin* OutPin)
{
	if (SachiParser_CopyJSONString(InDict, "label", &OutPin->Label, FALSE) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiParser_CopyJSONString(InDict, "name", &OutPin->Name, TRUE) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	char* S = NULL;
	if (SachiParser_GetJSONString(InDict, "mode", &S, TRUE) != SACHI_OK)
	{
		return SACHI_ERROR;
	}
	
	if (Sachi_PinModeFromString(S, &OutPin->Mode) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (SachiParser_GetJSONString(InDict, "side", &S, TRUE) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	if (Sachi_PinSideFromString(S, &OutPin->Side) != SACHI_OK)
	{
		return SACHI_ERROR;
	}

	return SACHI_OK;
}

// Copy an array of pins from JSON
static void* SachiParser_NewPins_Delegate(sachi_size_t InLength)
{
	return (void*)Sachi_NewPinWithLength(InLength);
}

static int SachiParser_CopyPins_Delegate(cJSON* InDict, void* InPins, sachi_size_t InIndex)
{
	return SachiParser_PinFromJSON(InDict, &((Sachi_Pin*)InPins)[InIndex]);
}

static void SachiParser_DeletePins_Delegate(void* InPins, sachi_size_t InSize)
{
	Sachi_DeletePinWithLength(&((Sachi_Pin*)InPins), InSize);
}

static int SachiParser_NodeFromJSON(cJSON* InDict, Sachi_Node** OutNode)
{
	Sachi_Node* Node = Sachi_NewNode();
	if (!Node)
	{
		SachiParser_SetErrorMemoryAllocation();
		return SACHI_ERROR;
	}

	if (SachiParser_CopyJSONString(InDict, "name", &Node->Name, TRUE) != SACHI_OK)
	{
		goto fail;
	}

	if (SachiParser_ArrayFromJSON(InDict, "pins", &Node->Pins, &Node->NumPins, &SachiParser_NewPins_Delegate, &SachiParser_CopyPins_Delegate, &SachiParser_DeletePins_Delegate, FALSE) != SACHI_OK)
	{
		goto fail;
	}

	*OutNode = Node;
	return SACHI_OK;

fail:
	Sachi_DeleteNode(Node);
	return SACHI_ERROR;
}

SACHI_PUBLIC(Sachi_Node*) Sachi_Load(const char* InFilename)
{
	char* Buffer = NULL;
	sachi_size_t Size = 0;
	if (Sachi_ReadFile(InFilename, &Buffer, &Size) != SACHI_OK)
	{
		SachiParser_SetErrorReadFile();
		return NULL;
	}

	return Sachi_LoadsWithLength(Buffer, Size);
}

SACHI_PUBLIC(Sachi_Node*) Sachi_Loads(const char* InBuffer)
{
	if (!InBuffer)
	{
		SachiParser_SetErrorNULL();
		return NULL;
	}

	return Sachi_LoadsWithLength(InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(Sachi_Node*) Sachi_LoadsWithLength(const char* InBuffer, sachi_size_t InBufferLength)
{
	// First, parse JSON dict
	cJSON* Dict = cJSON_ParseWithLength(InBuffer, InBufferLength);
	if (!Dict)
	{
		// Recuperate cJSON error
		Sachi_SetErrorPtr(cJSON_GetErrorPtr());
		return NULL;
	}

	// Convert from JSON to node.
	Sachi_Node* Node = NULL;
	SachiParser_NodeFromJSON(Dict, &Node);
	cJSON_Delete(Dict);

	return Node;
}
