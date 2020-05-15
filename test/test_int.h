#ifndef TEST_INT_H
#define TEST_INT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/interpreter.h"
#include "sachi/object/int.h"
#include "sachi/object/bool.h"

#ifdef __cplusplus
}
#endif

void test_int(Sachi_Interpreter* InInterpreter)
{
	Sachi_Object* Int = Sachi_NewInt(InInterpreter);
	assert(Int != NULL);
	assert(Int->Type != NULL);
	assert(Int->Interpreter != NULL);
	assert(SachiInt_Data(Int) == 0);
	Sachi_IntType.Delete(Int);

	Int = Sachi_NewIntFromValue(InInterpreter, 1);
	assert(SachiInt_Data(Int) == 1);
	Sachi_IntType.Delete(Int);
}

#endif