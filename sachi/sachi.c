#include "sachi/sachi.h"
#include "sachi/posix.h"

typedef struct {
	const char* message;
	size_t position;
} error;
static error global_error = { NULL, 0 };

SACHI_PUBLIC(const char*) Sachi_GetErrorPtr(void)
{
	return (const char*)(global_error.message + global_error.position);
}

SACHI_PUBLIC(void) Sachi_SetErrorPtr(const char* InBuffer)
{
	global_error.message = InBuffer;
	global_error.position = 0;
}

SACHI_PUBLIC(void) SachiError_SetMemoryAllocation()
{
	Sachi_SetErrorPtr("memory allocation failed");
}

SACHI_PUBLIC(LONG) Sachi_HashFromBuffer(const char* InBuffer)
{
	return Sachi_HashFromBufferAndLength(InBuffer, sachi_strlen(InBuffer));
}

SACHI_PUBLIC(LONG) Sachi_HashFromBufferAndLength(const char* InBuffer, sachi_size_t InLength)
{
	LONG result = 0x55555555;

	for (sachi_size_t I = 0; I < InLength; ++I)
	{
		result ^= *InBuffer++;
		result <<= 5;
	}

	return result;
}

#ifdef WITH_MEMORY_TRACKER
#include "sachi/object/object.h"
#include "sachi/object/list.h"

Sachi_MemoryTracker MemoryTracker = {
	NULL,
	NULL
};

SACHI_PUBLIC(void) Sachi_TrackObject(Sachi_Object* InObject)
{
	Sachi_MemoryTracker* Tracker = &MemoryTracker;
	while (Tracker->Next != NULL)
	{
		Tracker = Tracker->Next;
		if (Tracker->Object == InObject)
		{
			return;
		}
	}

	Sachi_MemoryTracker* Next = (Sachi_MemoryTracker*)sachi_malloc(sizeof(Sachi_MemoryTracker));
	Tracker->Next = Next;
	Next->Next = NULL;
	Next->Object = InObject;
}

SACHI_PUBLIC(void) Sachi_UntrackObject(Sachi_Object* InObject)
{
	Sachi_MemoryTracker* Tracker = &MemoryTracker;
	while (Tracker->Next)
	{
		if (Tracker->Next->Object == InObject)
		{
			Sachi_MemoryTracker* Next = Tracker->Next;
			Tracker->Next = Next->Next;
			Next->Next = NULL;
			Next->Object = NULL;
			sachi_free(Next);
			return;
		}
		Tracker = Tracker->Next;
	}
}
#endif

SACHI_PUBLIC(int) Sachi_ReadFile(const char* InFilename, char** OutBuffer, sachi_size_t* OutSize)
{
	if (!InFilename)
	{
		return SACHI_ERROR;
	}

	sachi_FILE* F = sachi_fopen(InFilename, "rb");
	if (!F)
	{
		return SACHI_ERROR;
	}

	sachi_fseek(F, 0, sachi_SEEK_END);
	sachi_size_t Size = (sachi_size_t)sachi_ftell(F);
	sachi_fseek(F, 0, sachi_SEEK_SET);
	char* Buffer = (char*)sachi_malloc((LONG)sizeof(char) * (Size + 1));
	if (!Buffer)
	{
		return SACHI_ERROR;
	}

	*OutSize = Size + 1;
	*OutBuffer = Buffer;

	sachi_fread((void*)Buffer, sizeof(char), Size, F);
	sachi_fclose(F);
	Buffer[Size] = '\0';
	return SACHI_OK;
}