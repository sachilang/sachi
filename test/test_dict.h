#ifndef TEST_DICT_H
#define TEST_DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "sachi/interpreter.h"
#include "sachi/object/dict.h"
#include "sachi/object/bool.h"
#include "sachi/object/string.h"

#ifdef __cplusplus
}
#endif

void test_dict(Sachi_Interpreter* InInterpreter)
{
	// Create dict
	Sachi_Object* Dict = Sachi_DictType.New(InInterpreter);
	Sachi_IncRef(Dict);
	assert(Dict != NULL);
	assert(Dict->Type != NULL);
	assert(Dict->Interpreter != NULL);
	assert(SachiDict_Size(Dict) == 0);
	assert(Sachi_IsTrue(SachiDict_Empty(Dict)));

	Sachi_Object* Key1 = Sachi_NewStringFromBuffer(InInterpreter, "1");
	Sachi_Object* Key2 = Sachi_NewStringFromBuffer(InInterpreter, "2");
	Sachi_IncRef(Key1);
	Sachi_IncRef(Key2);

	SachiDict_SetItem(Dict, Key1, Sachi_True);
	assert(SachiDict_Size(Dict) == 1);
	assert(Sachi_IsFalse(SachiDict_Empty(Dict)));
	SachiDict_SetItem(Dict, Key1, Sachi_False);
	assert(SachiDict_Size(Dict) == 1);
	SachiDict_SetItem(Dict, Key2, Sachi_False);
	assert(SachiDict_Size(Dict) == 2);
	Sachi_Object* Item = NULL;
	assert(SachiDict_GetItem(Dict, Key1, &Item) == SACHI_OK);
	assert(SachiDict_GetItem(Dict, Key2, &Item) == SACHI_OK);

	assert(SachiDict_RemoveItem(Dict, Key1, &Item) == SACHI_OK);
	assert(SachiDict_RemoveItem(Dict, Key2, &Item) == SACHI_OK);
	assert(SachiDict_Size(Dict) == 0);
	assert(Sachi_IsTrue(SachiDict_Empty(Dict)));

	SachiDict_SetItem(Dict, Key1, Sachi_True);
	SachiDict_SetItem(Dict, Key2, Sachi_True);
	SachiDict_Clear(Dict);
	assert(SachiDict_Size(Dict) == 0);
	assert(Sachi_IsTrue(SachiDict_Empty(Dict)));

	Sachi_DecRef(Key1);
	Sachi_DecRef(Key2);
	Sachi_DecRef(Dict);
}

#endif