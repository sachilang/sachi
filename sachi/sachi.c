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