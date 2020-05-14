#ifndef SACHI_POSIX_H
#define SACHI_POSIX_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sachi_size_t size_t

#define sachi_strlen(s) strlen(s)
#define sachi_strcmp(s1, s2) strcmp(s1, s2)

#define sachi_malloc(size) malloc(size)
#define sachi_realloc(ptr, size) realloc(ptr, size)
#define sachi_free(o) free(o)
#define sachi_memcpy(dest, source, length) memcpy(dest, source, length)

#ifdef SACHI_LOG_ENABLED
#define sachi_printf(format, ...) printf(format, __VA_ARGS__)
#endif
#define sachi_snprintf(buff, count, format, ...) snprintf(buff, count, format, __VA_ARGS__)

#define sachi_fopen(name, mode) fopen(name, mode)
#define sachi_ftell(f) ftell(f)
#define sachi_fread(buff, size, count, f) fread(buff, size, count, f)
#define sachi_fseek(f, offset, origin) fseek(f, offset, origin)
#define sachi_fclose(f) fclose(f)
#define sachi_FILE FILE
#define sachi_SEEK_END SEEK_END
#define sachi_SEEK_SET SEEK_SET

#ifndef LONG
#define LONG long
#endif

#ifndef BOOL
#define BOOL unsigned char
#endif

#ifndef TRUE
#define TRUE 0
#endif

#ifndef FALSE
#define FALSE 1
#endif

#ifdef __cplusplus
}
#endif

#endif