#ifndef TEST_INT_H
#define TEST_INT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "test/test_case.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/object/interpreter.h"
#include "sachi/object/int.h"
#include "sachi/object/bool.h"

#ifdef __cplusplus
}
#endif

void test_int(Sachi_Interpreter* InInterpreter)
{
	Sachi_Object* Int = Sachi_NewInt(InInterpreter);
	Assert(Int != NULL);
	Assert(Int->Type != NULL);
	Assert(Int->Interpreter != NULL);
	Assert(SachiInt_Data(Int) == 0);
	Sachi_DecRef(Int);

	Int = Sachi_NewIntFromValue(InInterpreter, 1);
	Assert(SachiInt_Data(Int) == 1);
	Sachi_DecRef(Int);
}

#endif