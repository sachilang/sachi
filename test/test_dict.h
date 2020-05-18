#ifndef TEST_DICT_H
#define TEST_DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "test/test_case.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/object/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/object/string.h"

#ifdef __cplusplus
}
#endif

void test_dict(Sachi_Interpreter* InInterpreter)
{
	// Create dict
	Sachi_Object* Dict = Sachi_DictType.New(InInterpreter);
	Assert(Dict != NULL);
	Assert(Dict->Type != NULL);
	Assert(Dict->Interpreter != NULL);
	Assert(SachiDict_Size(Dict) == 0);
	Assert(SachiDict_Empty(Dict));

	Sachi_Object* Value = Sachi_NewStringFromBuffer(InInterpreter, "value");
	Sachi_Object* Key1 = Sachi_NewStringFromBuffer(InInterpreter, "1");
	Sachi_Object* Key2 = Sachi_NewStringFromBuffer(InInterpreter, "2");

	SachiDict_SetItem(Dict, Key1, Value);
	Assert(SachiDict_Size(Dict) == 1);
	Assert(!SachiDict_Empty(Dict));
	SachiDict_SetItem(Dict, Key1, Value);
	Assert(SachiDict_Size(Dict) == 1);
	SachiDict_SetItem(Dict, Key2, Value);
	Assert(SachiDict_Size(Dict) == 2);
	Sachi_Object* Item = NULL;
	Assert(SachiDict_GetItem(Dict, Key1, &Item) == SACHI_OK);
	Assert(SachiDict_GetItem(Dict, Key2, &Item) == SACHI_OK);

	Assert(SachiDict_RemoveItem(Dict, Key1, &Item) == SACHI_OK);
	Assert(SachiDict_RemoveItem(Dict, Key2, &Item) == SACHI_OK);
	Assert(SachiDict_Size(Dict) == 0);
	Assert(SachiDict_Empty(Dict));

	SachiDict_SetItem(Dict, Key1, Value);
	SachiDict_SetItem(Dict, Key2, Value);
	SachiDict_Clear(Dict);
	Assert(SachiDict_Size(Dict) == 0);
	Assert(SachiDict_Empty(Dict));

	Sachi_DecRef(Key1);
	Sachi_DecRef(Key2);
	Sachi_DecRef(Value);
	Sachi_DecRef(Dict);
}

#endif