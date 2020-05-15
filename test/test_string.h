#ifndef TEST_STRING_H
#define TEST_STRING_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/interpreter.h"
#include "sachi/object/string.h"
#include "sachi/object/bool.h"

#ifdef __cplusplus
}
#endif

void test_string(Sachi_Interpreter* InInterpreter)
{
#define _SACHI_TEST_STRING "hello world !"

	// Create string
	Sachi_Object* String = Sachi_NewStringFromBuffer(InInterpreter, _SACHI_TEST_STRING);
	assert(String != NULL);
	assert(String->Type != NULL);
	assert(String->Interpreter != NULL);
	assert(SachiString_Size(String) == strlen(_SACHI_TEST_STRING));
	assert(Sachi_IsFalse(SachiString_Empty(String)));

	// Destroy
	Sachi_StringType.Delete(String);
}

#endif