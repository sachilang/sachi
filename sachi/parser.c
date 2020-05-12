#include "sachi/parser.h"
#include "sachi/posix.h"
#include "sachi/module.h"
#include "cJSON/cJSON.h"

typedef struct {
	const char* message;
	size_t position;
} error;
static error global_error = { NULL, 0 };

SACHI_PUBLIC(const char*) SachiParser_GetErrorPtr(void)
{
	return (const char*)(global_error.message + global_error.position);
}

static void SachiParser_SetErrorNULL()
{
	global_error.message = "invalid NULL pointer";
}

static void SachiParser_SetErrorJSONKeyNotFound()
{
	global_error.message = "non existent JSON key";
}

static void SachiParser_SetErrorNotJSONArray()
{
	global_error.message = "expected a JSON array";
}

static void SachiParser_SetErrorMemoryAllocation()
{
	global_error.message = "memory allocation failed";
}

static int SachiParser_CopyString(cJSON* InDict, const char* InName, char** OutBuffer)
{
	cJSON* Item = cJSON_GetObjectItem(InDict, InName);
	if (!Item || !cJSON_IsString(Item))
	{
		SachiParser_SetErrorJSONKeyNotFound();
		return SACHI_ERROR;
	}

	sachi_size_t Size = sachi_strlen(Item->valuestring);
	*OutBuffer = (char*)sachi_malloc(sizeof(char) * Size);
	sachi_memcpy(*OutBuffer, Item->valuestring, Size);

	return SACHI_OK;
}

static int SachiParser_CopyNode(cJSON* InDict, Sachi_Node* OutNode);
static int SachiParser_PinsFromJSON(cJSON* InDict, const char* InName, Sachi_NodePin** OutPins, sachi_size_t* OutSize);

// Copy Node informations from JSON dict.
static int SachiParser_CopyNode(cJSON* InDict, Sachi_Node* OutNode)
{
	if (SachiParser_CopyString(InDict, "name", &OutNode->Name) != SACHI_OK)
		return SACHI_ERROR;
	if (SachiParser_PinsFromJSON(InDict, "inputs", &OutNode->Inputs, &OutNode->NumInputs) != SACHI_OK)
		return SACHI_ERROR;
	if (SachiParser_PinsFromJSON(InDict, "outputs", &OutNode->Outputs, &OutNode->NumOutputs) != SACHI_OK)
		return SACHI_ERROR;

	return SACHI_OK;
}

static int SachiParser_PinFromJSON(cJSON* InDict, Sachi_NodePin* OutPin)
{
	if (SachiParser_CopyString(InDict, "name", &OutPin->Name) != SACHI_OK)
		return SACHI_ERROR;

	return SACHI_OK;
}

// Copy an array of JSON items.
// New = a lambda creating a new generic item
// Copy = a lambda copying a generic item from JSON
static int SachiParser_ArrayFromJSON(cJSON* InDict, const char* InName, void** OutObjects, sachi_size_t* OutSize, void*(*New)(sachi_size_t), int(*Copy)(cJSON*, void*, sachi_size_t))
{
	cJSON* List = cJSON_GetObjectItem(InDict, InName);
	if (List && !cJSON_IsArray(List))
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
		Sachi_DeleteNodePinWithLength(Objects, Size);
	}
	return SACHI_ERROR;
}

// Copy an array of pins from JSON
static void* SachiParser_NewPins_Delegate(sachi_size_t InLength)
{
	return (void*)Sachi_NewNodePinWithLength(InLength);
}

static int SachiParser_CopyPins_Delegate(cJSON* InDict, void* InPins, sachi_size_t InIndex)
{
	return SachiParser_PinFromJSON(InDict, &((Sachi_NodePin*)InPins)[InIndex]);
}

static int SachiParser_PinsFromJSON(cJSON* InDict, const char* InName, Sachi_NodePin** OutPins, sachi_size_t* OutSize)
{
	return SachiParser_ArrayFromJSON(InDict, InName, (void**)OutPins, OutSize, &SachiParser_NewPins_Delegate, &SachiParser_CopyPins_Delegate);
}

// Convert the JSON dict to Node.
static Sachi_Node* SachiParser_NodeFromJSON(cJSON* InDict)
{
	Sachi_Node* Node = Sachi_NewNode();
	if (SachiParser_CopyNode(InDict, Node) != SACHI_OK)
	{
		Sachi_DeleteNode(Node);
		return NULL;
	}

	return Node;
}

SACHI_PUBLIC(int) SachiParser_ReadFile(const char* InFilename, char** OutBuffer, sachi_size_t* OutSize)
{
	if (!InFilename)
	{
		SachiParser_SetErrorNULL();
		return SACHI_ERROR;
	}

	sachi_FILE* F = sachi_fopen(InFilename, "rb");
	if (!F)
	{
		global_error.message = "file not found";
		return SACHI_ERROR;
	}

	sachi_fseek(F, 0, sachi_SEEK_END);
	sachi_size_t Size = (sachi_size_t)sachi_ftell(F);
	sachi_fseek(F, 0, sachi_SEEK_SET);
	char* Buffer = (char*)sachi_malloc((LONG)sizeof(char) * (Size + 1));
	if (!Buffer)
	{
		SachiParser_SetErrorMemoryAllocation();
		return SACHI_ERROR;
	}

	*OutSize = Size + 1;
	*OutBuffer = Buffer;

	sachi_fread((void*)Buffer, sizeof(char), Size, F);
	sachi_fclose(F);
	Buffer[Size] = '\0';
	return SACHI_OK;
}

SACHI_PUBLIC(Sachi_Node*) SachiParser_Load(const char* InFilename)
{
	char* Buffer = NULL;
	sachi_size_t Size = 0;
	if (SachiParser_ReadFile(InFilename, &Buffer, &Size) != SACHI_OK)
	{
		return NULL;
	}

	return SachiParser_LoadsWithLength(Buffer, Size);
}

SACHI_PUBLIC(Sachi_Node*) SachiParser_Loads(const char* InBuffer)
{
	if (!InBuffer)
	{
		SachiParser_SetErrorNULL();
		return NULL;
	}

	return SachiParser_LoadsWithLength(InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(Sachi_Node*) SachiParser_LoadsWithLength(const char* InBuffer, sachi_size_t InBufferLength)
{
	if (!InBuffer)
	{
		SachiParser_SetErrorNULL();
		return NULL;
	}

	cJSON* Dict = cJSON_ParseWithLength(InBuffer, InBufferLength);
	if (!Dict)
	{
		global_error.message = cJSON_GetErrorPtr();
		return NULL;
	}

	Sachi_Node* Node = SachiParser_NodeFromJSON(Dict);
	cJSON_Delete(Dict);

	return Node;
}
